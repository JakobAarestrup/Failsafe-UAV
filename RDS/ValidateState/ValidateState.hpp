#pragma once
#include <iostream>
class ValidateState
{

public:
  ValidateState();
  ~ValidateState();
  void GetGPSValues();
  void GetIMUValues();
  void GetBaroValues();

  void UpdateSystemValues();
  void LogData();

  void AxisControl();
  void RouteControl();
  void HeightControl();
  void landDrone();

private:
  /**
   * @brief GPS Values
   *
   */

  float longitudeSYS_, latitudeSYS_, longPoleSYS_, latPoleSYS_;
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
  float longitudeRDS_, latitudeRDS_, longPoleRDS_, latPoleRDS_;
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
