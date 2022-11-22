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
  RDS(ValidateState* state) : state_(nullptr) {
    TransitionTo(state);
    /*

    */
  }

  ~RDS() { delete state_; }

  void TransitionTo(ValidateState* state) {
    if (state_) delete state_;
    state_ = state;
    state_->setRDS_(this);
  }

  void AnalyseAxis() { state_->AxisControl(); }
  void AnalyseRoute() { state_->RouteControl(); }
  void AnalyseHeight() { state_->HeightControl(); }

  void UpdateSystemValues()
  {
    /* get
    get
    get
    get */
  }
  
  void LogData()
  {
    /*START logging*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n"); // GPS Data + baro
    printf("Roll: %f \370 Pitch: %f \370  Yaw: %f \370 \n");  // IMU Data
  }

private:
  ValidateState* state_;

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

};

class Normal : public ValidateState {
public:
  void AxisControl() override {
    std::cout << typeid(this).name() << " called AxisControl()\n";
  }
  void RouteControl() override {
    std::cout << typeid(this).name() << " called RouteControl()\n";
  }
  void HeightControl() override {
    std::cout << typeid(this).name() << " called HeightControl()\n";
  }
};

class Critical : public ValidateState {
public:
  void AxisControl() override {
    std::cout << typeid(this).name() << " called AxisControl()\n";
    //RDS_->TransitionTo(new Normal);
  }
  void RouteControl() override {
    std::cout << typeid(this).name() << " called RouteControl()\n";
  }
  void HeightControl() override {
    std::cout << typeid(this).name() << " called HeightControl()\n";
    RDS_->TransitionTo(new Normal);
  }
};

class HyperCritical : public ValidateState {
public:
  void AxisControl() override {
    std::cout << typeid(this).name() << " Yo yocalled AxisControl()\n";
    //RDS_->TransitionTo(new Normal);
  }
  void RouteControl() override {
    std::cout << typeid(this).name() << " called RouteControl()\n";
  }
  void HeightControl() override {
    std::cout << typeid(this).name() << " called HeightControl()\n";
    //
  }

};

