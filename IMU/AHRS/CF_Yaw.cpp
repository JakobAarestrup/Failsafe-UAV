#include "CF_Yaw.h"

CF_Yaw::CF_Yaw(float tau, float TS)
{
    f_mx.setup(tau,TS,1.0f);
    f_my.setup(tau,TS,1.0f);
    f_g.setup(tau,TS,tau);
    
    }
CF_Yaw::~CF_Yaw() {}

void CF_Yaw::update(float gz, float mx, float my, float mz)
{
    c1 =  cosf(data.est_RPY[0]);
    c2 =  cosf(data.est_RPY[1]);
    s1 =  sinf(data.est_RPY[0]);
    s2 =  sinf(data.est_RPY[1]);
    
    m  << mx,my,mz;
    R  << c2,0,c1*s2,0,c1,-s1,-s2,c2*s1,c1*c2;
    m = R*m;
    // careful: -sign at atan2: the Magnetometer measures world frame
    // if copter turns CCW, the Magnet vector turns CW!!
    data.est_RPY[2] = uw2pi(-atan2(f_my(m(1)),f_mx(m(0))) + f_g(gz));
    
    }    