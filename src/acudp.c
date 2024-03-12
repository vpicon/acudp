#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "acudp.h"
#include "types.h"
#include "format.h"

int acudp_init(acudp_handle **handle_ptr, const char* server_address, short server_port)
{
    acudp_handle *acudp = (acudp_handle *) malloc(sizeof(acudp_handle));
    if (!acudp) {
        return ACUDP_ERROR;
    }

    // Set up the server address
    memset(&acudp->server_address, 0, sizeof(acudp->server_address));
    acudp->server_address.sin_family      = AF_INET;                 // ipv4
    acudp->server_address.sin_port        = htons(server_port);
    inet_pton(AF_INET, server_address, &(acudp->server_address.sin_addr.s_addr));

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

    format_setup_response_from_data(resp, buf);

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


int acudp_read_spot_event(acudp_handle *acudp, acudp_lap_t *lap)
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

    format_lap_from_data(lap, buf);
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
