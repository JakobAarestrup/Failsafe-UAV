#include "ValidateState.hpp"
#include <stdio.h>
#include <unistd.h>
#include "IMU/I2C.hpp"
#include <sys/time.h>
#include <time.h>
#include <vector>

/**
 * Includes from telemetry example code
 */
#include <iostream>
#include <future>
#include <memory>
#include <thread>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

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

int mymillis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

void mainloop(ValidateState RDS, GPS NEO, BAR Barometer, Telemetry &telemetry)
{
    while (1)
    {
        int startofloop = mymillis();
        RDS.UpdateSystemValues(NEO, Barometer);                    // gets all values from sensors
        const Telemetry::Position position = telemetry.position(); // returns struct with values from baro and GPS
        // const Telemetry::EulerAngle euler = telemetry.attitude_euler(); // returns struct with euler angles
        /*Sets all values from MAVLINK*/
        RDS.SetMAVLinkValues(position.relative_altitude_m, position.latitude_deg, position.longitude_deg,
                             euler.roll_deg, euler.pitch_deg, euler.yaw_deg);
        RDS.LogData();       // Sends sensor data to log file
        RDS.AxisControl();   // Checks for Failure on the Axises
        RDS.RouteControl();  // Checks for Failure in the KML
        RDS.HeightControl(); // Checks for Failure for height
        while (mymillis() - startofloop < 100)
        {
        }
        printf("Loop Time %d\n", mymillis() - startofloop);
    }
}

void updateIMUValues(ValidateState RDS, IMU IMU)
{
    while (1)
    {
        RDS.GetIMUValues(IMU);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main(int argc, char **argv)
{
    /**
     * @brief RDS initialized
     *
     */
    ValidateState RDS; // System

    char NONE[1];

    if (argc < 3) // minimum arguments 2
    {
        RDS.Usage(argv[0]);
        return 1;
    }

    /* cout << "You have entered " << argc
         << " arguments:"
         << "\n";
    */
    for (int i = 0; i < argc; ++i)
    {
        std::cout << argv[i] << " plads: " << i << std::endl;
    }
    std::cout << "\n";
    std::cout << argv[1] << "\n";
    std::cout << argv[4] << "\n"
              << std::endl;

    if ((strcmp(argv[1], argv[4]) == 0)) // if you write 20 on serial it
    {
        RDS.configValidateState(argv[2], argv[3], NONE, NONE);
    }
    else
    {
        RDS.configValidateState(argv[2], argv[3], argv[4], argv[5]);
    }

    /**
     * @brief Initialize used object variables
     *
     */
    GPS G1;
    I2C I1;
    // IMU IMU1;
    IMU IMU2;
    BAR B1;
    // B1.calibrateBAR();
    /**
     * @brief  Configuration of Sensors
     *
     */
    /* G1.configAll();     // configs the GPS
     I1.initializeI2C(); // Initialize IMU2 right now but will do both */
    G1.configAll();     // configs the GPS
    I1.initializeI2C(); // Initialize IMU2 right now but will do both
    /**
     * @brief Calibration..
     *
     */
    //  IMU1.calibrateGyro(1);
    IMU2.calibrateGyro(2);

    /**
     * @brief MAVLINK connection.
     *
     */
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

    const auto set_rate_position = telemetry.set_rate_position(1.0);
    if (set_rate_position != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_position << '\n';
        return 1;
    }

    /*   const auto set_rate_euler = telemetry.set_rate_attitude_euler(1.0);
      if (set_rate_euler != Telemetry::Result::Success)
      {
          std::cerr << "Setting rate failed: " << set_rate_euler << '\n';
          return 1;
      } */

    /**
     * @brief starting two threads to do main loop and get the IMU data
     */
    std::vector<std::thread> threads;

    threads.push_back(std::thread(mainloop, std::ref(RDS), std::ref(G1), std::ref(B1), std::ref(telemetry)));
    threads.push_back(std::thread(updateIMUValues, std::ref(RDS), std::ref(IMU2)));

    for (auto &th : threads)
    {
        th.join();
    }

    return 0;
}