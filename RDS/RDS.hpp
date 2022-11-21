#pragma once

#include "ValidateState.hpp"

class ValidateState;

class RDS {
    friend class ValidateState;

public:
    RDS();
    ~RDS();
    /* void AxisControl(RDS &RDS);
  	void RouteControl(RDS &RDS);
  	void HeightControl(RDS &RDS); */
    //void GetSensorData();
    //.. Fylde RDS op med alle værdierne.. en get function eller sådan noget?
    void LogData(); 

private:
    unsigned int stock;
    ValidateState *state;
};