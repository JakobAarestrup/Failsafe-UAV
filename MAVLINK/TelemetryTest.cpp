#include <iostream>
#include <unistd.h>
#include <mavsdk/mavsdk.h>

int main()
{
    mavsdk::Mavsdk mavsdk;
    // ConnectionResult conn_result = mavsdk.add_udp_connection();
    //  Wait for the system to connect via heartbeat
    while (mavsdk.systems().size() == 0)
    {
        usleep(1000000);
    }

    // System got discovered.
    auto system = get_system(mavsdk);
    /* // System got discovered.
    System system = mavsdk.systems()[0]; */
    return 0;
}

/*  Mavsdk mavsdk;
   ConnectionResult connection_result = mavsdk.add_udp_connection();

   if (connection_result != ConnectionResult::Success) {
       std::cerr << "Connection failed: " << connection_result << '\n';
       return 1; */