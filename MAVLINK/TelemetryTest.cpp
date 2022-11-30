#include <iostream>
#include <unistd.h>
#include <mavsdk/mavsdk.h>
#include <future>

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
    Mavsdk::NewSystemHandle handle = mavsdk.subscribe_on_new_system([&mavsdk, &prom, &handle]()
                                                                    {
        auto system = mavsdk.systems().back();

        if (system->has_autopilot()) {
            std::cout << "Discovered autopilot\n";

            // Unsubscribe again as we only want to find one system.
            mavsdk.unsubscribe_on_new_system(handle);
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
    ConnectionResult conn_result = mavsdk.add_udp_connection();

    if (conn_result != ConnectionResult::Success)
    {
        std::cerr << "Connection failed: " << conn_result << '\n';
        return 1;

        //  Wait for the system to connect via heartbeat

        while (mavsdk.systems().size() == 0)
        {
            usleep(1000000);
        }

        auto system = get_system(mavsdk);
        if (!system)
        {
            return 1;
        }
        // System got discovered.
        // mavsdk::System system = mavsdk.systems()[0];
        return 0;
    }
