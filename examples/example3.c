#include <stdio.h>
#include <stdlib.h>
#include "acudp.h"

/**
 * This example connects client to AC Server subscribing it
 * as for spot events (lap info). Prints several events on
 * standard output.
 */

void print_lap_info(const acudp_lap_t *c) {
    printf("identifier: %d\n", c->car_identifier_number);
    printf("lap: %d\n", c->lap);
    printf("driver_name: %s\n", c->driver_name);
    printf("car_name: %s\n", c->car_name);
    printf("time milliseconds: %d\n", c->time_ms);
}


int main() {
    acudp_handle *acudp;
    int rc;  // return code


    if ((rc = acudp_init(&acudp)) != ACUDP_OK)
        exit(1);

    acudp_setup_response_t setup_response;
    acudp_send_handshake(acudp, &setup_response);
    acudp_client_subscribe(acudp, ACUDP_SUBSCRIPTION_SPOT);

    while (1) {
        acudp_lap_t data;
        rc = acudp_read_spot_event(acudp, &data);
        if (rc == ACUDP_CLI_SUB)
            perror("cli sub");
        print_lap_info(&data);
    }

    acudp_exit(acudp);
    return 0;
}
