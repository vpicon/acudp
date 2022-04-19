#ifndef _LIB_ASSETTO_CORSA_UDP_HPP
#define _LIB_ASSETTO_CORSA_UDP_HPP

extern "C" {
#include "acudp.h"
}

namespace acudp {

enum class SubscribeMode {
    update,  // car events
    spot,    // lap events
};

// TODO: Define exceptions thrown depending on return codes
struct AcudpException{};


class ACUDP {
 private:
    acudp_handle *_acudp; 
    bool _subscribed;

 public:
    ACUDP() 
    {
        acudp_init(&_acudp);
    }

    ~ACUDP() 
    {
        if (_subscribed)
            send_dismiss();

        acudp_exit(_acudp);
    }

    // Public Interface
    inline acudp_setup_response_t send_handshake() 
    {
        acudp_setup_response_t response;
        acudp_send_handshake(_acudp, &response);
        return response;
    }

    inline void subscribe(const SubscribeMode mode) 
    {
        acudp_client_subscription_t 
        subscribe = (mode == SubscribeMode::update)
                ? ACUDP_SUBSCRIPTION_UPDATE
                : ACUDP_SUBSCRIPTION_SPOT;
        acudp_client_subscribe(_acudp, subscribe);
        _subscribed = true;
    }

    inline acudp_car_t read_update_event() 
    {
        acudp_car_t car;
        acudp_read_update_event(_acudp, &car);
        return car;
    }

    inline acudp_lap_t read_spot_event() 
    {
        acudp_lap_t lap;
        acudp_read_spot_event(_acudp, &lap);
        return lap;
    }

    inline void send_dismiss() 
    {
        acudp_send_dismiss(_acudp);
        _subscribed = false;
    }
};

} // namespace acudp

#endif  // _LIB_ASSETTO_CORSA_UDP_HPP
