#include "SimpleState.hpp"
//#include "Neo7.hpp"


int main() {
  RDS Dronesystem(new Normal); // Default State
  //GPS::configAll(); // configs the GPS
  //BARO::configAll(); // configs the BARO
  //IMU::configAll(); // configs the IMU


  // while(1)
  //{
  /* Dronesystem.UpdateSystemValues();   // gets all values from sensors
  Dronesystem.LogData();*/               // Sends data to log file

  Dronesystem.AnalyseAxis(); // Checks for Failure on the Axises
  Dronesystem.AnalyseRoute(); // Checks for Failure in the KML
  Dronesystem.AnalyseHeight(); // Checks for Failure for height

 // } 

  return 0;
}