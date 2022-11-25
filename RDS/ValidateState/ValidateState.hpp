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
  int SatellitesSYS_ = 0;

  /**
   * @brief Baro Values
   *
   */
  float altitudeSYS_ = 0;

  /**
   * @brief IMU Values
   *
   */
  float RollSYS_ = 0, PitchSYS_ = 0, YawSYS_ = 0;

  /**
   * @brief GPS Values
   *
   */
  float longitudeRDS_ = 0, latitudeRDS_ = 0;
  char longPoleRDS_[1] = 0, latPoleRDS_[1] = 0;
  int SatellitesRDS_ = 0;

  /**
   * @brief Baro Values
   *
   */
  float altitudeRDS_ = 0;

  /**
   * @brief IMU Values
   *
   */
  float RollRDS_ = 0, PitchRDS_ = 0, YawRDS_ = 0;

  int state_ = 0;
};
