#ifndef EKF_RP_H_
#define EKF_RP_H_

#include <mbed.h>
#include "Eigen/Dense.h"

using namespace Eigen;

class EKF_RP
{
public:

    EKF_RP(float Ts);

    virtual ~EKF_RP();
    
    void reset();
    void increase_diag_P();
    float get_est_state(uint8_t i);
    void update(float gyro_x, float gyro_y, float accel_x, float accel_y);
    
private:
    
    float s1;
    float c1;
    float s2;
    float c2;
    
    float scale_P0;
    float g;
    float kv;
    float Ts;
    float rho;
    Matrix <float, 2, 1>  var_gy;
    Matrix <float, 6, 1>  var_fx; 

    Matrix <float, 2, 1>  u;
    Matrix <float, 2, 1>  y;
    Matrix <float, 6, 1>  x;
    Matrix <float, 6, 6>  F;
    Matrix <float, 2, 6>  H;
    Matrix <float, 6, 6>  Q;
    Matrix <float, 2, 2>  R;
    Matrix <float, 6, 6>  P;
    Matrix <float, 6, 2>  K;
    Matrix <float, 6, 6>  I;
    Matrix <float, 2, 1>  e;
        
    void update_angles();
    void calc_F();
    void calc_H();
    void initialize_R();
    void initialize_Q();
    void calc_Q();
    
    Matrix <float, 6, 1> fxd();
    Matrix <float, 2, 1> gy();
    
};

#endif