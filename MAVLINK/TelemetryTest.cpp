#include <iostream>
#include "../MAVSDK/src/mavsdk/core/include/mavsdk/mavsdk.h"
#include "../MAVSDK/src/mavsdk/plugins/telemetry/include/plugins/telemetry/telemetry.h"
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

    return 0;
}
