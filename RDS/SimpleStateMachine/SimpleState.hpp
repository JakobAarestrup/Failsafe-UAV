#pragma once
#include <iostream>
#include <typeinfo>

class RDS; // forward declaration

class ValidateState {

public:
  virtual ~ValidateState() {}

  void setRDS_(RDS* RDS) {
    RDS_ = RDS;
  }

  virtual void AxisControl() = 0;
  virtual void RouteControl() = 0;
  virtual void HeightControl() = 0;

protected:
  RDS* RDS_;
};

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

private:
  ValidateState* state_;
  /**
   * @brief GPS Values
   * 
   */
   /**
   * @brief GPS Values
   * 
   */

  float longitudeSYS_ = 0, latitudeSYS_ = 0, longPoleSYS_= 0, latPoleSYS_ = 0;
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

};

class Normal : public ValidateState{
public:
  virtual void AxisControl();
  virtual void RouteControl();
  virtual void HeightControl();
};

class Critical : public ValidateState {
public:
  virtual void AxisControl();
  virtual void RouteControl();
  virtual void HeightControl();
  void landDrone();
}; 

/* class HyperCritical : public ValidateState {
public:
  virtual void AxisControl();
  virtual void RouteControl();
  virtual void HeightControl();
  void landDrone();
};
 */

