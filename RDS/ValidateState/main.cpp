#include "ValidateState.hpp"
#include <stdio.h>
#include <unistd.h>
#include "IMU/I2C.hpp"
#include <sys/time.h>
#include <time.h>

int mymillis()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

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

  /**
   * @brief  Configuration of Sensors
   *
   */
  G1.configAll();     // configs the GPS
  I1.initializeI2C(); // Initialize IMU1
  // I2.initializeI2C(); // Initialize IMU2
  /**
   * @brief Calibration..
   *
   */
  B1.calibrateBAR();
  IMU2.calibrateGyro(2);
  // IMU2.calibrateGyro();

  /**
   * @brief Used variables
   *
   */
  int startofloop;

  /**
   * @brief starting of loop for checking and logging sensor data
   *
   */
  while (1)
  {
    startofloop = mymillis();
    RDS.UpdateSystemValues(G1, B1, IMU2); // gets all values from sensors

    RDS.LogData();       // Sends sensor data to log file
    RDS.AxisControl();   // Checks for Failure on the Axises
    RDS.RouteControl();  // Checks for Failure in the KML
    RDS.HeightControl(); // Checks for Failure for height
    // N++;

    while (mymillis() - startofloop < 20)
    {
      usleep(100);
    }

    printf("Loop Time %d\n", mymillis() - startofloop);

    /*   if (N > 5)
      {
        break;
      } */
  }
  return 0;
}