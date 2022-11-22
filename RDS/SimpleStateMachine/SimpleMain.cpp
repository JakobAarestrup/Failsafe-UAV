#include "SimpleState.hpp"
#include "HyperCritical.hpp"

int main() {
  RDS Dronesystem(new HyperCritical); // Default State

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