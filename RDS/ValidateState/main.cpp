#include "ValidateState.hpp"
#include <stdio.h>

int main()
{
  ValidateState RDS; // System
  FILE *fp;
  GPS G1;

  /*Configuration of Sensors*/
  G1.configAll(); // configs the GPS
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
    RDS.UpdateSystemValues(); // gets all values from sensors

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