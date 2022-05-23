# ACUDP Library
Library for obtaining telemetry data from AssettoCorsa UDP interface.
The library is based in a C API, and also provides a C++ wrapper API,
and a Python extension module.

## Build
To compile and build the C/C++ library the it is required to have installed `gcc` and GNU `make` in the system. For the Python extension, `Python3` must be in the system.

To build the entire library, write the command:
```bash
$ make
```
This will actually build the C library binary into the `lib/` directory as a static library. The Python extension module will also be built into the `python-extension/build/` directory.


## Usage
This section gives some brief usage of the library for the three API's provided. Further usage examples can be found in the `examples/` directory.

### C API
The example includes the library header in `include/`. Then it initializes the library, subscribes it to receive car info real-time events, and then closes the communication after reading one event.

```c
#include "acudp.h"

int main() 
{
    // Init library
    acudp_handle *acudp;
    acudp_init(&acudp));

    // Subscribe to car info events
    acudp_setup_response_t setup_response;
    acudp_send_handshake(acudp, &setup_response);
    acudp_client_subscribe(acudp, ACUDP_SUBSCRIPTION_UPDATE);

    // Read car data
    acudp_car_t data;
    acudp_read_update_event(acudp, &data);
    /* ... process car data ... */

    // Close communication and exit library
    acudp_send_dismiss(acudp);
    acudp_exit(acudp);
    return 0;
}
```

For simplicity, no error-checking is done. All error codes can be found in the library header file.

### C++ API
The example includes the library header in `include/`. And performs the same actions as in the C example.

```c++
#include "acudp.hpp"

int main() 
{
    // Initailise ACUDP instance and connect to server
    acudp::ACUDP acudp{};
    acudp.send_handshake();

    // Subscribe to car info events
    acudp.subscribe(acudp::SubscribeMode::update);
    
    // Read car data
    auto car = acudp.read_update_event();
    /* ... process car data ... */

    // All resources are freed by RAII. No need to manually disconnect.
    return 0;
}
```

Again, for simplicity, little exceptions are checked. All exceptions can be found in the library header file.

### Python API
The example performs the same actions as in the previous examples.

```python
import acudp

if __name__ == "__main__":
    # Initailise ACUDP instance and connect to server
    client = acudp.ACUDP()
    client.send_handshake()

    # Subscribe to car info events
    client.subscribe_to_updates()

    # Read car data
    car_info = acudp.read_update_event();
    ## ... process car_info data ... ##

    # Disconnect client
    client.send_dismiss()
```

Again, for simplicity, little exceptions are checked. 

## Documentation
All the documentation can be found in the [DOCUMENTATION.md](docs/DOCUMENTATION.md) file.

## Planned
The main missing functionality is to add support for either: 
* pull data at real time (which may cause buffer and data misalignment, due to consuming 
data at a lower rate than the server is producing it), or 
* read data in parallel, and consume actualized data on demand.
