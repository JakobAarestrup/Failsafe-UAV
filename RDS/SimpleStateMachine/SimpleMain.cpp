#include "SimpleState.hpp"

int main() {
  RDS Dronesystem(new Normal); // Default State

  // while(1)
  //{
  /* Dronesystem.UpdateSystemValues(); 
  Dronesystem.LogData(); */

  Dronesystem.AnalyseAxis();
  Dronesystem.AnalyseRoute();
  Dronesystem.AnalyseHeight();

 // } 

  return 0;
}