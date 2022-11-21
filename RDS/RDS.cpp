// RDS.cpp
#include "RDS.hpp"
#include "ValidateState.hpp"

RDS::RDS() 
{
    static_cast<ValidateState *>(new Normal()); // Default State
}

RDS::~RDS() { delete state; }

void RDS::LogData()
{
    /*START logging*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n"); // GPS Data + baro
    printf("Roll: %f \370 Pitch: %f \370  Yaw: %f \n");  // IMU Data
}