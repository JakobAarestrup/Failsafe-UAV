#pragma once
#include <iostream>
#include "NEO7/Neo7.hpp"
#include <string.h>
#include <fstream>
#include <sstream>
class ValidateState
{

public:
  ValidateState();
  ~ValidateState();
  void GetGPSValues(GPS NEO);
  void GetIMUValues();
  void GetBaroValues();

  void UpdateSystemValues(GPS NEO);
  void LogData();

  void AxisControl();
  void RouteControl();
  void HeightControl();
  void landDrone();
  inline void Logger(std::string logMsg);
  inline std::string getCurrentDateTime(std::string s);

private:
  /**
   * @brief GPS Values
   *
   */

  float longitudeSYS_, latitudeSYS_;
  char longPoleSYS_, latPoleSYS_;
  int SatellitesSYS_;

  /**
   * @brief Baro Values
   *
   */
  float altitudeSYS_;

  /**
   * @brief IMU Values
   *
   */
  float RollSYS_, PitchSYS_, YawSYS_;

  /**
   * @brief GPS Values
   *
   */
  float longitudeRDS_, latitudeRDS_;
  char longPoleRDS_, latPoleRDS_;
  int SatellitesRDS_;

  /**
   * @brief Baro Values
   *
   */
  float altitudeRDS_;

  /**
   * @brief IMU Values
   *
   */
  float RollRDS_, PitchRDS_, YawRDS_;

  int state_;
};
