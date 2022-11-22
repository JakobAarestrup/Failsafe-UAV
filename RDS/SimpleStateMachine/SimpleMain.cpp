#include "SimpleState.hpp"
//#include "Neo7.hpp"


int main() {
  RDS Dronesystem(new HyperCritical); // Default State
  //GPS::configAll(); // configs the GPS
  //BARO::configAll(); // configs the GPS
  //IMU::configAll(); // configs the GPS


  // while(1)
  //{
  /* Dronesystem.UpdateSystemValues();  // gets all values from sensors
  Dronesystem.LogData(); // Sends data to log file */

  Dronesystem.AnalyseAxis();
  Dronesystem.AnalyseAxis();
  Dronesystem.AnalyseRoute();
  Dronesystem.AnalyseHeight();

 // } 

  return 0;
}