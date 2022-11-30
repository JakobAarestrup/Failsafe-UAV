#include <iostream>
#include <unistd.h>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <future>

using namespace mavsdk;

int main()
{
    Mavsdk mavsdk;
    ConnectionResult conn_result = mavsdk.add_udp_connection();

    if (conn_result != ConnectionResult::Success)
    {
        std::cerr << "Connection failed: " << conn_result << '\n';
        return 1;
    }
    //  Wait for the system to connect via heartbeat

    while (mavsdk.systems().size() == 0)
    {
        usleep(1000000);
    }

    auto system = mavsdk.systems().back();
    if (!system)
    {
        return 1;
    }
    // System got discovered.
    // mavsdk::System system = mavsdk.systems()[0];
    return 0;
}
