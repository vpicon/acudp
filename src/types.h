#ifndef _LIB_ASSETTO_CORSA_UDP_TYPES_H
#define _LIB_ASSETTO_CORSA_UDP_TYPES_H

#include "acudp.h"


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



#endif  // _LIB_ASSETTO_CORSA_UDP_TYPES_H
