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


void print_car_info(const acudp_car_t& car) {
    std::cout << car.speed_kmh << std::endl;
    std::cout << car.lap_time << std::endl;
    std::cout << car.car_position_normalized << std::endl;
    std::cout << car.lap_count << std::endl;
}


int main() {
    acudp::ACUDP acudp;
    auto setup_response = acudp.send_handshake();
    print_setup_response(setup_response);
    acudp.subscribe(acudp::SubscribeMode::update);

    while (true) {
        auto car = acudp.read_update_event();
        print_car_info(car);
    }

    return 0;
}
