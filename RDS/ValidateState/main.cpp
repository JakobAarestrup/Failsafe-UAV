#include "ValidateState.hpp"
#include <stdio.h>
#include "NEO7/Neo7.hpp"

int main()
{
  ValidateState RDS; // System
  FILE *fp;

  /*Configuration of Sensors*/
  // GPS::configAll(); // configs the GPS
  // BARO::configAll(); // configs the BARO
  // I1.initializeI2C(); // IMU1
  // I2.initializeI2C(); // IMU2

  /*Calibration*/
  /* IMU1.calibrateGyro();
    IMU2.calibrateGyro();*/

  /*Logging initialized*/
  fp = freopen("RDSLog.txt", "w", stdout);
  int N = 0;

  while (1)
  {
    /* Dronesystem.UpdateSystemValues();*/ // gets all values from sensors

    RDS.LogData();       // Sends sensor data to log file
    RDS.AxisControl();   // Checks for Failure on the Axises
    RDS.RouteControl();  // Checks for Failure in the KML
    RDS.HeightControl(); // Checks for Failure for height
    N++;
    if (N > 100)
    {
      break;
    }
  }
  fclose(fp); // close logging
  return 0;
}