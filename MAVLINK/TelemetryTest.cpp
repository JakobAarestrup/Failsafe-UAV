#include <iostream>
#include <../MAVSDK/src/mavsdk/core/include/mavsdk/mavsdk.h>

int main()
{
    Mavsdk mavsdk;
    ConnectionResult conn_result = mavsdk.add_udp_connection();
    // Wait for the system to connect via heartbeat
    while (mavsdk.system().size() == 0)
    {
        sleep_for(seconds(1));
    }
    // System got discovered.
    System system = mavsdk.systems()[0];

    return 0;
}
