#ifndef CF_YAW_H_
#define CF_YAW_H_

#include <mbed.h>
#include "Eigen/Dense.h"
#include "Data_Logger.h"
#include "Read_Xtern_Sensors.h"
#include "IIR_filter.h"
#include "Unwrapper_2pi.h"

extern DATA_Xchange data;

using namespace Eigen;


class CF_Yaw
{
public:

    CF_Yaw(float,float);

    virtual ~CF_Yaw();
    void update(float,float,float,float);
private:
    float s1,s2,c1,c2;
    Matrix <float, 3, 3>  R;
    Matrix <float, 3, 1>  m;
    IIR_filter f_g,f_mx,f_my;
    Unwrapper_2pi uw2pi;

};

#endif