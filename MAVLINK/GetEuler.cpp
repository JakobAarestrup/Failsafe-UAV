#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

using namespace mavsdk;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        usage(argv[0]);
        return 1;
    }

    Mavsdk mavsdk;
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success)
    {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    // Create telemetry object
    auto telemetry = std::make_shared<Telemetry>(mavsdk);

    // Subscribe to euler angles
    telemetry->euler_angle_async([](Telemetry::EulerAngle euler_angle)
                                 {
    // Print out euler angles
    std::cout << "Euler Angles: roll = " << euler_angle.roll_deg << ", pitch = " << euler_angle.pitch_deg
              << ", yaw = " << euler_angle.yaw_deg << std::endl; });

    // Keep program running until it is manually stopped
    while (true)
    {
        sleep_for(seconds(1));
    }

    return 0;
}