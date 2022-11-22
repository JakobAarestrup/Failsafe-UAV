#pragma once

class RDS {

public:
  RDS(ValidateState* state);
 
  ~RDS(); 

  void GetGPSValues();
  void GetIMUValues();
  void GetBaroValues();

  void UpdateSystemValues();
  void LogData();

  void TransitionTo(ValidateState* state);

  void AnalyseAxis();
  void AnalyseRoute();
  void AnalyseHeight();

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
   * @brief IMU Values
   * 
   */
  float RollRDS_, PitchRDS_, YawRDS_;

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

private:
  ValidateState* state_;
  /**
   * @brief GPS Values
   * 
   */

};
