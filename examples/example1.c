#include <stdio.h>
#include <stdlib.h>
#include "acudp.h"

/**
 * This example performs hanshake with AC server
 * and prints on screen the response.
 */

void print_setup_response(const acudp_setup_response_t *resp) {
    printf("car name: %s\n", resp->car_name);
    printf("driver name: %s\n", resp->driver_name);
    printf("identifier: %d\n", resp->identifier);
    printf("version: %d\n", resp->version);
    printf("track name: %s\n", resp->track_name);
}


int main() {
    acudp_handle *acudp;
    int rc;  // return code

    if ((rc = acudp_init(&acudp, "127.0.0.1", 9996)) != ACUDP_OK)
        exit(1);

    acudp_setup_response_t setup_response;
    acudp_send_handshake(acudp, &setup_response);
    print_setup_response(&setup_response);

    acudp_exit(acudp);
    return 0;
}
