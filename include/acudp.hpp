#ifndef _LIB_ASSETTO_CORSSA_UDP_HPP
#define _LIB_ASSETTO_CORSSA_UDP_HPP

extern "C" {
#include "acudp.h"
}


class ACUDP {
 private:
    acudp_handle *_acudp; 

 public:
    ACUDP() {
        acudp_init(&_acudp);
    }

    ~ACUDP() {
        acudp_exit(_acudp);
    }

    // Public Interface
    inline acudp_setup_response_t send_handshake() {
        acudp_setup_response_t response;
        acudp_send_handshake(_acudp, &response);
        return response;
    }
};


#endif  // _LIB_ASSETTO_CORSSA_UDP_HPP
