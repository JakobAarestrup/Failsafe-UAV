#pragma once
#include <iostream>
#include "NEO7/Neo7.hpp"
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
  inline void Logger(string logMsg);
  inline string getCurrentDateTime(string s);

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
