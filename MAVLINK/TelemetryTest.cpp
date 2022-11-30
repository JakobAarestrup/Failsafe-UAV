#include <iostream>
#include <unistd.h>
#include <mavsdk/mavsdk.h>
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
    auto telemetry = Telemetry{system};

    const Telemetry::Result set_rate_result = telemetry.set_rate_position(1.0);
    if (set_rate_result != Telemetry::Result::Success)
    {
        // handle rate-setting failure (in this case print error)
        std::cout << "Setting rate failed:" << set_rate_result << '\n';
    }

    telemetry.subscribe_position([](Telemetry::Position position)
                                 { std::cout << "Altitude: " << position.relative_altitude_m << " m" << std::endl
                                             << "Latitude: " << position.latitude_deg << std::endl
                                             << "Longitude: " << position.longitude_deg << '\n'; });
    // System got discovered.
    // mavsdk::System system = mavsdk.systems()[0];
    return 0;
}
