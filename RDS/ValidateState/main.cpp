#include "ValidateState.hpp"
//#include "Neo7.hpp"


int main() {
  ValidateState Dronesystem; // System
  //GPS::configAll(); // configs the GPS
  //BARO::configAll(); // configs the BARO
  //IMU::configAll(); // configs the IMU


  // while(1)
  //{
  /* Dronesystem.UpdateSystemValues();   // gets all values from sensors
  Dronesystem.LogData();*/               // Sends data to log file

  Dronesystem.AxisControl(); // Checks for Failure on the Axises
  Dronesystem.RouteControl(); // Checks for Failure in the KML
  Dronesystem.HeightControl(); // Checks for Failure for height

 // } 

  return 0;
}