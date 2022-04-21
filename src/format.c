#include <string.h>
#include "format.h"


/**
 * Reads buffer formatted as 100 byte len array of shorts,
 * terminated by 0x0025, into str as string.
 */
void _read_data_string(char *str, const char *buf)
{
    int nread = 0;
    while (nread < 50) {
        if (*buf == 0x25) {
            *str = '\0';
            break;
        }

        *str++ = *buf++;
        buf++;            // skip 0x00
        nread++;
    }

    if (nread == 50) *--str = '\0'; // Always nul terminate
}


/**
 * Reads little-endian int stored in buf, and copies it into n.
 */
void _read_data_int(int *n, const char *buf)
{
    memcpy(n, buf, sizeof(int));
}


void format_setup_response_from_data(acudp_setup_response_t *resp, const char *buf)
{
    _read_data_string(resp->car_name,    buf);
    _read_data_string(resp->driver_name, buf + 100);
    _read_data_int(  &resp->identifier,  buf + 2 * 100);
    _read_data_int(  &resp->version,     buf + 2 * 100 + 4);
    _read_data_string(resp->track_name,  buf + 2 * 100 + 2 * 4);
}


void format_lap_from_data(acudp_lap_t *lap, const char *buf)
{
    _read_data_int(  &lap->car_identifier_number,  buf);
    _read_data_int(  &lap->lap,                    buf + 4);
    _read_data_string(lap->driver_name,            buf + 2*4);
    _read_data_string(lap->car_name,               buf + 100 + 2*4);
    _read_data_int(  &lap->time_ms,                buf + 2*100 + 2*4);
}
