#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "acudp.h"


const short ACSERVER_DEFAULT_PORT = 9996;

/**
 * Opaque stucture for the library state handle definitions
 */
enum acudp_setup_operation {
    ACUDP_SETUP_HANDSHAKE        = 0,
    ACUDP_SETUP_SUBSCRIBE_UPDATE = 1,
    ACUDP_SETUP_SUBSCRIBE_SPOT   = 2,
    ACUDP_SETUP_DISMISS          = 3,
};


struct acudp_handle_s {
    struct sockaddr_in server_address;
    int sockfd;
    acudp_client_subscription_t subscription;
};


int acudp_init(acudp_handle **handle_ptr)
{
    acudp_handle *acudp = (acudp_handle *) malloc(sizeof(acudp_handle));
    if (!acudp) {
        return ACUDP_ERROR;
    }

    // Set up the server address
    memset(&acudp->server_address, 0, sizeof(acudp->server_address));
    acudp->server_address.sin_family      = AF_INET;                 // ipv4
    acudp->server_address.sin_port        = htons(ACSERVER_DEFAULT_PORT);
    acudp->server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // server in localhost

    // Create a datagram socket in the internet domain and use the // default protocol (UDP).
    if ((acudp->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        free(acudp);
        return ACUDP_SOCK;
    }

    acudp->subscription = ACUDP_SUBSCRIPTION_NONE;

    *handle_ptr = acudp;
    return ACUDP_OK;
}


int acudp_exit(acudp_handle *acudp)
{
    // Close socket
    close(acudp->sockfd);

    // Free allocated memory for handle
    free(acudp);

    return ACUDP_OK;
}


int _acudp_send_setup_struct(acudp_handle *acudp,
        const acudp_setup_t *setup)
{
    ssize_t nwritten = sendto(acudp->sockfd,
            setup, sizeof(*setup),
            0,
            (struct sockaddr *) &acudp->server_address,
            sizeof(acudp->server_address));
    if (nwritten < 0) {
        return ACUDP_ERROR;
    }

    return ACUDP_OK;
}


/**
 * Reads buffer formatted as 100 byte len array of shorts,
 * terminated by 0x0025, into dest as string.
 */
void _read_data_string(char *dest, const char *buf) {
    int nread = 0;
    while (nread < 50) {
        if (*buf == 0x25) {
            *dest = '\0';
            break;
        }

        *dest++ = *buf++; // copy buf into dest
        buf++;            // skip 0x00
        nread++;
    }

    if (nread == 50) *--dest = '\0'; // Always nul terminate
}


void _read_data_int(int *n, const char *buf) {
    memcpy(n, buf, sizeof(int));
}


void _acudp_setup_response_from_data(acudp_setup_response_t *resp, const char *buf)
{
    _read_data_string(resp->car_name,    buf);
    _read_data_string(resp->driver_name, buf + 100);
    _read_data_int(  &resp->identifier,  buf + 2 * 100);
    _read_data_int(  &resp->version,     buf + 2 * 100 + 4);
    _read_data_string(resp->track_name,  buf + 2 * 100 + 2 * 4);
}


int _acudp_recv_setup_response_struct(acudp_handle *acudp,
        acudp_setup_response_t *resp)
{
    char buf[100*4 + 4*2];   // Response data buffer
    ssize_t nread = recvfrom(acudp->sockfd,
            buf, sizeof(buf),
            0, NULL, NULL);

    if (nread != sizeof(buf)) {
        return ACUDP_ERROR;
    }

    _acudp_setup_response_from_data(resp, buf);

    return ACUDP_OK;
}


int acudp_send_handshake(acudp_handle *acudp,
        acudp_setup_response_t *resp)
{
    // Send hanshake setup
    acudp_setup_t handshake = {
        .identifier=1,
        .version=1,
        .operation_id=ACUDP_SETUP_HANDSHAKE
    };
    _acudp_send_setup_struct(acudp, &handshake);

    // Receive response from server
    _acudp_recv_setup_response_struct(acudp, resp);

    return ACUDP_OK;
}


int acudp_client_subscribe(acudp_handle *acudp,
        acudp_client_subscription_t subscription)
{
    if (subscription != ACUDP_SUBSCRIPTION_UPDATE
            && subscription != ACUDP_SUBSCRIPTION_SPOT) {
        return ACUDP_INV_ARG;
    }

    int operation_id = (subscription == ACUDP_SUBSCRIPTION_UPDATE)
                     ? ACUDP_SETUP_SUBSCRIBE_UPDATE
                     : ACUDP_SETUP_SUBSCRIBE_SPOT;
    acudp_setup_t setup = {
        .identifier=1,
        .version=1,
        .operation_id=operation_id
    };

    int ret = _acudp_send_setup_struct(acudp, &setup);
    if (ret == ACUDP_OK)  // Update subscription state on success
        acudp->subscription = subscription;

    return ret;
}


int acudp_read_update_event(acudp_handle *acudp, acudp_car_t *data)
{
    // Check proper subscription state
    if (acudp->subscription != ACUDP_SUBSCRIPTION_UPDATE)
        return ACUDP_CLI_SUB;

    // Read sockfd buffer for packet containing car_t data
    ssize_t nread = recvfrom(acudp->sockfd,
            data, sizeof(*data),
            0, NULL, NULL);

    if (nread != sizeof(*data)) {
        return ACUDP_ERROR;
    }

    // Check format of incoming data
    if (data->identifier != 'a' || data->size != sizeof(acudp_car_t)) {
        return ACUDP_FMT;
    }

    return ACUDP_OK;
}


void _acudp_lap_from_data(acudp_lap_t *data, const char *buf)
{
    _read_data_int(  &data->car_identifier_number,  buf);
    _read_data_int(  &data->lap,                    buf + 4);
    _read_data_string(data->driver_name,            buf + 2*4);
    _read_data_string(data->car_name,               buf + 100 + 2*4);
    _read_data_int(  &data->time_ms,                buf + 2*100 + 2*4);
}


int acudp_read_spot_event(acudp_handle *acudp, acudp_lap_t *data)
{
    // Check proper subscription state
    if (acudp->subscription != ACUDP_SUBSCRIPTION_SPOT)
        return ACUDP_CLI_SUB;

    // Read sockfd buffer for packet containing car_t data
    char buf[100*2 + 4*3];   // Lap data buffer
    ssize_t nread = recvfrom(acudp->sockfd,
            buf, sizeof(buf),
            0, NULL, NULL);

    if (nread != sizeof(buf)) {
        return ACUDP_ERROR;
    }

    _acudp_lap_from_data(data, buf);
    return ACUDP_OK;
}


int acudp_send_dismiss(acudp_handle *acudp)
{
    // Dismiss setup
    acudp_setup_t dismiss = {
        .identifier=1,
        .version=1,
        .operation_id=ACUDP_SETUP_DISMISS
    };
    return _acudp_send_setup_struct(acudp, &dismiss);
}
