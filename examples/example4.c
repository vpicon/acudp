#include <stdio.h>
#include <stdlib.h>
#include "acudp.h"

/**
 * This example connects client to AC Server subscribing it
 * as for update events (Car info). Prints several events on
 * standard output.
 */

void print_car_info(const acudp_car_t *c) {
    printf("identifier: %c\n", c->identifier);
    printf("data size: %d\n", c->size);
    printf("speed_kmh: %f\n", c->speed_kmh);
    printf("lap_time: %d\n", c->lap_time);
    printf("lap_count: %d\n", c->lap_count);
    printf("gas: %f\n", c->gas);
    printf("brake: %f\n", c->brake);
    printf("steer: %f\n", c->steer);
    printf("car_position_normalized: %f\n", c->car_position_normalized);
    printf("car_coordinates: (%f, %f, %f)\n", c->car_coordinates[0],  c->car_coordinates[1],  c->car_coordinates[2]);
}


int main() {
    acudp_handle *acudp;
    int rc;  // return code


    if ((rc = acudp_init(&acudp)) != ACUDP_OK)
        exit(1);

    acudp_setup_response_t setup_response;
    acudp_send_handshake(acudp, &setup_response);
    acudp_client_subscribe(acudp, ACUDP_SUBSCRIPTION_UPDATE);

    int n = 4;
    while (n-- > 0) {
        acudp_car_t data;
        rc = acudp_read_update_event(acudp, &data);
        if (rc == ACUDP_CLI_SUB)
            perror("cli sub");
        print_car_info(&data);
    }

    acudp_send_dismiss(acudp);

    // No more events should arrive
    n = 4;
    while (n-- > 0) {
        acudp_car_t data;
        rc = acudp_read_update_event(acudp, &data);
        if (rc == ACUDP_CLI_SUB)
            perror("cli sub");
        print_car_info(&data);
    }

    acudp_exit(acudp);
    return 0;
}
