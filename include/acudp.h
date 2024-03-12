#ifndef _LIB_ASSETTO_CORSA_UDP_H
#define _LIB_ASSETTO_CORSA_UDP_H

#include <stdint.h>


/**
 * Constants
 */
extern const short ACSERVER_DEFAULT_PORT;


/**
 * Opaque stucture for the library state handle
 */
struct acudp_handle_s; 
typedef struct acudp_handle_s acudp_handle; 


enum acudp_errors {
    ACUDP_OK = 0,      // Success
    ACUDP_ERROR,       // Generic error
    ACUDP_INV_ARG,     // Invalid argument
    ACUDP_CLI_SUB,     // Client not subscribed accordingly
    ACUDP_SOCK,        // Error opening socket
    ACUDP_FMT,         // Data format error
};


typedef struct acudp_setup {
    int32_t identifier;
    int32_t version;
    int32_t operation_id;
} acudp_setup_t;


typedef struct acudp_setup_response {
    char    car_name[50];
    char    driver_name[50];
    int identifier;
    int version;
    char    track_name[50];
    char    track_config[50];
} acudp_setup_response_t;


typedef enum acudp_client_subscription {
    ACUDP_SUBSCRIPTION_UPDATE = 1,
    ACUDP_SUBSCRIPTION_SPOT = 2,
    ACUDP_SUBSCRIPTION_NONE
} acudp_client_subscription_t;


typedef struct acudp_car {
    char identifier;
    int  size;

    float speed_kmh;
    float speed_mph;
    float speed_ms;

    char is_abs_enabled;
    char is_abs_in_action;
    char is_tc_in_action;
    char is_tc_enabled;
    char is_in_pit;
    char is_engine_limiter_on;

    float accG_vertical;
    float accG_horizontal;
    float accG_frontal;

    int lap_time;
    int last_lap;
    int best_lap;
    int lap_count;

    float gas;
    float brake;
    float clutch;
    float engine_rpm;
    float steer;
    int   gear;
    float cg_height;

    float wheel_angular_speed[4];
    float slip_angle[4];
    float slip_angle_contact_patch[4];
    float slip_ratio[4];
    float tyre_slip[4];
    float nd_slip[4];
    float load[4];
    float Dy[4];
    float Mz[4];
    float tyre_dirty_level[4];

    float camber_RAD[4];
    float tyre_radius[4];
    float tyre_loaded_radius[4];
    float suspension_height[4];

    float car_position_normalized;
    float car_slope;
    float car_coordinates[3];
} acudp_car_t;


typedef struct acudp_lap {
    int  car_identifier_number;
    int  lap;
    char driver_name[50];
    char car_name[50];
    int  time_ms;
} acudp_lap_t;


/**
 * Initializes the api, and allocates all necessary resources.
 * Also allocates the acudp_handle returned by argument-value.
 * server_address is the IP or hostname of the AC server
 * server_port is the port that the AC server is running on
 * Return ACUDP_OK on success, otherwise the error code.
 */
int acudp_init(acudp_handle **acudp, const char* server_address, short server_port);
/**
 * Termination routine of the api, disconnects from the server.
 * Also frees the acudp_handle given.
 * Return ACUDP_OK on success, otherwise the error code.
 */
int acudp_exit(acudp_handle *acudp);


/**
 * Sends initial handshake to AC server and stores the setup response
 * into the given struct pointer.
 * Returns ACUDP_OK on success, error otherwise.
 */
int acudp_send_handshake(acudp_handle *acudp, acudp_setup_response_t *resp);

/**
 * Subscribes the client to the AC server as ACUDP_SUBSCRIPTION_UPDATE 
 * to get car info updates in real time, or as ACUDP_SUBSCRIPION_SPOT 
 * to get lap info.
 * Returns ACUDP_OK on success, error otherwise.
 */
int acudp_client_subscribe(acudp_handle *, acudp_client_subscription_t);

/**
 * Reads for incoming update event from AC Server, containing car
 * telemetry data, and stores it into the given car_t pointer.
 * Required for client subscription to be ACUDP_SUBSCRIPTION_UPDATE,
 * by calling on acudp_client_subscribe function.
 * Returns ACUDP_OK on success, error otherwise.
 */
int acudp_read_update_event(acudp_handle *, acudp_car_t *);

/**
 * Reads for incoming spot event from AC Server, containing lap
 * telemetry data, and stores it into the given lap_t pointer.
 * Required for client subscription to be ACUDP_SUBSCRIPTION_SPOT,
 * by calling on acudp_client_subscribe function.
 * Returns ACUDP_OK on success, error otherwise.
 */
int acudp_read_spot_event(acudp_handle *, acudp_lap_t *);

/**
 * Sends final dismiss to AC server which desuscribes the
 * client from receiving events fomr AC server.
 * Returns ACUDP_OK on success, error otherwise.
 */
int acudp_send_dismiss(acudp_handle *acudp);


#endif  // _LIB_ASSETTO_CORSA_UDP_H
