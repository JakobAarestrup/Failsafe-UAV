#include "ValidateState.hpp"
#include <stdio.h>
#include <unistd.h>
#include "IMU/I2C.hpp"
#include <sys/time.h>
#include <time.h>

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

/* int main(int argc, char **argv)
{
  ValidateState RDS; // System
    if (argc != 2)
    {
        RDS.usage(argv[0]);
        return 1;
    }
    RDS.configValidateState(argv[1]);
    */

int main()
{
  /**
   * @brief RDS initialized
   *
   */
  ValidateState RDS; // System

  /**
   * @brief Initialize used object variables
   *
   */
  GPS G1;
  I2C I1;
  IMU IMU2;
  // IMU IMU2;
  BAR B1;
  B1.calibrateBAR();
  /**
   * @brief  Configuration of Sensors
   *
   */
  G1.configAll();     // configs the GPS
  I1.initializeI2C(); // Initialize IMU1
  //  I2.initializeI2C(); // Initialize IMU2
  /**
   * @brief Calibration..
   *
   */
  // IMU2.calibrateGyro(2);
  //  IMU2.calibrateGyro();

  /**
   * @brief Used variables
   *
   */
  int startofloop;

  /**
   * @brief MAVLINK connection.
   *
   */
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

  const auto set_rate_position = telemetry.set_rate_position(1.0);
  if (set_rate_position != Telemetry::Result::Success)
  {
    std::cerr << "Setting rate failed: " << set_rate_position << '\n';
    return 1;
  }

  const auto set_rate_euler = telemetry.set_rate_attitude_euler(1.0);
  if (set_rate_euler != Telemetry::Result::Success)
  {
    std::cerr << "Setting rate failed: " << set_rate_euler << '\n';
    return 1;
  }

  /**
   * @brief starting loop for checking and logging sensor data
   *
   */
  while (1)
  {
    startofloop = mymillis();
    RDS.UpdateSystemValues(G1, B1, IMU2);                           // gets all values from sensors
    const Telemetry::Position position = telemetry.position();      // returns struct with values from baro and GPS
    const Telemetry::EulerAngle euler = telemetry.attitude_euler(); // returns struct with euler angles
    /*Sets all values from MAVLINK*/
    RDS.SetMAVLinkValues(position.relative_altitude_m, position.latitude_deg, position.longitude_deg,
                         euler.roll_deg, euler.pitch_deg, euler.yaw_deg);
    RDS.LogData();       // Sends sensor data to log file
    RDS.AxisControl();   // Checks for Failure on the Axises
    RDS.RouteControl();  // Checks for Failure in the KML
    RDS.HeightControl(); // Checks for Failure for height
                         // N++;

    while (mymillis() - startofloop < 200)
    {
    }
    printf("Loop Time %d\n", mymillis() - startofloop);
  }
  return 0;
}