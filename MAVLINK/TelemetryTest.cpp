#include <iostream>
#include <unistd.h>
#include <mavsdk/mavsdk.h>
using namespace mavsdk;

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

    int main()
    {
        Mavsdk mavsdk;
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