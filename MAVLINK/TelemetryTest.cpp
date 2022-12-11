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

void usage(const std::string &bin_name)
{
    std::cerr << "Usage : " << bin_name << " <connection_url>\n"
              << "Connection URL format should be :\n"
              << " For TCP : tcp://[server_host][:server_port]\n"
              << " For UDP : udp://[bind_host][:bind_port]\n"
              << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "For example, to connect to the simulator use URL: udp://:14540\n";
}

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

    const auto set_rate_result1 = telemetry.set_rate_attitude_euler(10.0);
    if (set_rate_result1 != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_result1 << '\n';
        return 1;
    }

    /* // Check until vehicle is ready to arm
    while (telemetry.health_all_ok() != true)
    {
        std::cout << "Telemetry is not okay\n";
        sleep_for(seconds(1));
    } */

    const Action::Result land_result = action.land();
    if (land_result != Action::Result::Success)
    {
        std::cerr << "Land failed: " << land_result << '\n';
        return 1;
    }

    float relative_alt;
    float longitude;
    float latitude;
    double roll;
    double pitch;
    double yaw;
    // telemetry.attitude_euler(Telemetry::EulerAngler euler);  // Set up callback to monitor altitude while the vehicle is in flight

    /* telemetry.subscribe_attitude_euler([](Telemetry::EulerAngle euler){

       std::cout << "Euler:     (" << euler.roll_deg << ", " << euler.pitch_deg << ", " << euler.yaw_deg << ")" << std::endl;}); */

    while (1)
    {
        const Telemetry::Position position = telemetry.position();
        relative_alt = position.relative_altitude_m;
        longitude = position.longitude_deg;
        latitude = position.latitude_deg;

        std::cout << "Altitude: " << relative_alt << " m" << std::endl
                  << "Latitude: " << longitude << std::endl
                  << "Longitude: " << latitude << '\n';

        const Telemetry::Euler euler = telemetry.attitude_euler();
        roll = euler.roll_deg;
        if (std::isnan(roll))
        {
            roll = 0.0f; // or any other default value
        }
        pitch = euler.pitch_deg;
        yaw = euler.yaw_deg;
        std::cout << "Euler:     (" << roll << ", " << pitch << ", " << yaw << ")" << std::endl;
        sleep_for(seconds(1));
    }

    return 0;
}