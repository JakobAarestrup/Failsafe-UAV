#pragma once
#include <iostream>
#include "NEO7/Neo7.hpp"
#include "IMU/IMU.hpp"
#include <string.h>
#include "BAR/BAR.hpp"
#include <fstream>
#include <sstream>

class ValidateState
{

public:
  ValidateState();
  ~ValidateState();
  void GetGPSValues(GPS NEO);
  void GetIMUValues(IMU sensor);
  void GetBaroValues(BAR barometer);

  void UpdateSystemValues(GPS NEO, BAR barometer, IMU Sensor);
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
  char longPoleSYS_[1], latPoleSYS_[1];
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
  char longPoleRDS_[1], latPoleRDS_[1];
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
