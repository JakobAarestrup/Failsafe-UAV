#pragma once

#include "RDS.hpp"
#include <exception>
#include <stdexcept>

class RDS;

class ValidateState {
public:
  virtual void AxisControl(RDS &RDS);
  virtual void RouteControl(RDS &RDS);
  virtual void HeightControl(RDS &RDS);

  virtual ~ValidateState();

protected:
  void setState(RDS &RDS, ValidateState *st); // changes the state
  virtual void GetDroneSensorData(RDS &RDS); // Mavlink
  virtual void LandDrone(RDS &RDS, unsigned int quantity); // Mavlink
};

class Normal : public ValidateState {
public:
  virtual void GetDroneSensorData(RDS &RDS); // Mavlink

  virtual void AxisControl(RDS &RDS);
  virtual void RouteControl(RDS &RDS);
  virtual void HeightControl(RDS &RDS);

  virtual ~Normal();
};

class Critical : public ValidateState {
public:
  virtual void GetDroneSensorData(RDS &RDS); // Mavlink

  virtual void AxisControl(RDS &RDS);
  virtual void RouteControl(RDS &RDS);
  virtual void HeightControl(RDS &RDS);
  virtual ~Critical();
};

class HyperCritical : public ValidateState {
public:
  void LandDrone(RDS &RDS, unsigned int quantity); // Mavlink
  virtual ~HyperCritical();
};

// --------