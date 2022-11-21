// RDS.cpp
#include "RDS.hpp"
#include "ValidateState.hpp"

RDS::RDS(unsigned int _stock) {
  stock = _stock;
  state = _stock > 0 ? static_cast<ValidateState *>(new Normal())
                    : static_cast<ValidateState *>(new Critical())
                    : static_cast<ValidateState *>(new HyperCritical();

                // laver bare den første??
}

RDS::~RDS() { delete state; }

void RDS::LogData()
{
    /*START logging*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n"); // GPS Data + baro
    printf("Roll: %f \370 Pitch: %f \370  Yaw: %f \n");  // IMU Data
}