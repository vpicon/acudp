#ifndef _LIB_ASSETTO_CORSA_UDP_FORMAT_H
#define _LIB_ASSETTO_CORSA_UDP_FORMAT_H

#include "acudp.h"


/**
 * Formats structs reading from the data buffers given by network.
 */
void format_setup_response_from_data(acudp_setup_response_t *resp, const char *buf);
void format_lap_from_data(acudp_lap_t *lap, const char *buf);


#endif  // _LIB_ASSETTO_CORSA_UDP_FORMAT_H
