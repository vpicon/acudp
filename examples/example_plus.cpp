#include <iostream>
#include "acudp.hpp"

/**
 * This example performs hanshake with AC server
 * and prints on screen the response.
 */

void print_setup_response(const acudp_setup_response_t& resp) {
    std::cout << resp.car_name << std::endl;
    std::cout << resp.driver_name << std::endl;
    std::cout << resp.identifier << std::endl;
    std::cout << resp.version << std::endl;
    std::cout << resp.track_name << std::endl;
}


int main() {
    ACUDP acudp;
    auto setup_response = acudp.send_handshake();
    print_setup_response(setup_response);
    return 0;
}
