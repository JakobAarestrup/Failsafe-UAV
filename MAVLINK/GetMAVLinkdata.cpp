#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>
#include <future>
#include <memory>
#include <thread>

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

std::shared_ptr<System> get_system(Mavsdk &mavsdk)
{
    std::cout << "Waiting to discover system...\n";
    auto prom = std::promise<std::shared_ptr<System>>{};
    auto fut = prom.get_future();

    // We wait for new systems to be discovered, once we find one that has an
    // autopilot, we decide to use it.
    mavsdk.subscribe_on_new_system([&mavsdk, &prom]()
                                   {
        auto system = mavsdk.systems().back();

        if (system->has_autopilot()) {
            std::cout << "Discovered autopilot\n";

            // Unsubscribe again as we only want to find one system.
            mavsdk.subscribe_on_new_system(nullptr);
            prom.set_value(system);
        } });

    // We usually receive heartbeats at 1Hz, therefore we should find a
    // system after around 3 seconds max, surely.
    if (fut.wait_for(seconds(3)) == std::future_status::timeout)
    {
        std::cerr << "No autopilot found.\n";
        return {};
    }

    // Get discovered system now.
    return fut.get();
}

int main()
{

    Mavsdk mavsdk;
    ConnectionResult connection_result = mavsdk.add_any_connection("serial:///dev/ttyS0:57600");

    if (connection_result != ConnectionResult::Success)
    {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = get_system(mavsdk);
    if (!system)
    {
        return 1;
    }

    // Instantiate plugins.
    auto telemetry = Telemetry{system};
    auto action = Action{system};

    // We want to listen to the altitude of the drone at 1 Hz.
    const auto set_rate_result = telemetry.set_rate_position(1.0);
    if (set_rate_result != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_result << '\n';
        return 1;
    }

    const auto set_rate_result1 = telemetry.set_rate_attitude_euler(1.0);
    if (set_rate_result1 != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_result1 << '\n';
        return 1;
    }

    // Set up callback to monitor altitude while the vehicle is in flight
    telemetry.subscribe_position([](Telemetry::Position position)
                                 { std::cout << "Altitude: " << position.relative_altitude_m << " m" << std::endl
                                             << "Latitude: " << position.latitude_deg << std::endl
                                             << "Longitude: " << position.longitude_deg << '\n'; });
    altitudeSYS_ = position.relative_altitude_m;
    longitudeSYS_ = position.latitude_deg;
    latitudeSYS_ = position.longitude_deg;

    telemetry.subscribe_attitude_euler([](Telemetry::EulerAngle euler)
                                       { std::cout << "Euler: (" << euler.roll_deg << ", " << euler.pitch_deg << ", " << euler.yaw_deg << ")" << std::endl; });

    RollSYS_ = euler.roll_deg;
    PitchSYS_ = euler.pitch_deg;
    YawSYS_ = euler.yaw_deg;
}