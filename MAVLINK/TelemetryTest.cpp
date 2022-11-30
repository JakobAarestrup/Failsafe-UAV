#include <iostream>
#include <unistd.h>
#include <mavsdk/mavsdk.h>

int main()
{
    mavsdk::Mavsdk mavsdk;
    mavsdk.add_udp_connection();
    // Wait for the system to connect via heartbeat
    while (mavsdk.systems().size() == 0)
    {
        usleep(1000000);
    }

    // System got discovered.
    mavsdk::System system = mavsdk.systems();
    return 0;
}
