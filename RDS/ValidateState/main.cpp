#include "ValidateState.hpp"
#include <stdio.h>
#include <unistd.h>

int mymillis()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

int main()
{
  ValidateState RDS; // System
  FILE *fp;
  GPS G1;
  IMU I1;
  // IMU I2;

  /*Configuration of Sensors*/
  G1.configAll(); // configs the GPS
  // BARO::configAll(); // configs the BARO
  I1.initializeI2C(); // IMU1
  // I2.initializeI2C(); // IMU2

  /*Calibration*/
  IMU1.calibrateGyro();
  // IMU2.calibrateGyro();

  /*Logging initialized*/
  // fp = freopen("RDSLog.txt", "w", stdout);
  int N = 0;
  int startofloop;

  while (1)
  {
    startofloop = mymillis();
    RDS.UpdateSystemValues(G1); // gets all values from sensors

    RDS.LogData();       // Sends sensor data to log file
    RDS.AxisControl();   // Checks for Failure on the Axises
    RDS.RouteControl();  // Checks for Failure in the KML
    RDS.HeightControl(); // Checks for Failure for height
    N++;

    while (mymillis() - startofloop < 20)
    {
      usleep(100);
    }

    printf("Loop Time %d\n", mymillis() - startofloop);

    if (N > 5)
    {
      break;
    }
  }
  // fclose(fp); // close logging
  return 0;
}