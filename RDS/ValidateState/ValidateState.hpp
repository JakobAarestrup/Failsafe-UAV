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
  void Usage(const std::string &bin_name);
  void configValidateState();
  void GetGPSValues(GPS NEO);
  void GetIMUValues(IMU sensor);
  void GetBaroValues(BAR barometer);
  void SetMAVLinkValues(float alt, float lng, float lat, float roll, float pitch, float yaw);

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

  float longitudeSYS_ = 0, latitudeSYS_ = 0;
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
  char longPoleRDS_[1] = "", latPoleRDS_[1] = "";
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

  /*
  compareAlt_ = 300 // default value
  compareRoll_ = 300 // default value
  comparePitch_ = 300 // default value
  compareYaw_ = 300 // default value
  compareLong_
  compareLat_
  compareVelocity

  */
};
