#ifndef EKF_RPY_H_
#define EKF_RPY_H_

#include <mbed.h>
#include "Eigen/Dense.h"

using namespace Eigen;

class EKF_RPY
{
public:

    EKF_RPY(float Ts, float mx0, float my0, float mz0);
    EKF_RPY(float Ts);

    virtual ~EKF_RPY();
    
    void set_para();
    void reset();
    void increase_diag_P();
    void set_m0(float mx0, float my0, float mz0);
    float get_est_state(uint8_t i);
    void update(float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float magnet_x, float magnet_y);
    
private:

    Matrix <float,  3,  1>  m0;
    
    float s1;
    float c1;
    float s2;
    float c2;
    float s3;
    float c3;
    
    float scale_P0;
    float g;
    float kv;
    float Ts;
    float rho;
    Matrix <float, 4, 1>  var_gy;
    Matrix <float, 8, 1>  var_fx; 

    Matrix <float, 3, 1>  u;
    Matrix <float, 4, 1>  y;
    Matrix <float, 8, 1>  x;
    Matrix <float, 8, 8>  F;
    Matrix <float, 4, 8>  H;
    Matrix <float, 8, 8>  Q;
    Matrix <float, 4, 4>  R;
    Matrix <float, 8, 8>  P;
    Matrix <float, 8, 4>  K;
    Matrix <float, 8, 8>  I;
    Matrix <float, 4, 1>  e;
        
    void update_angles();
    void calc_F();
    void calc_H();
    void initialize_R();
    void initialize_Q();
    void calc_Q();
    
    Matrix <float, 8, 1> fxd();
    Matrix <float, 4, 1> gy();
    
};

#endif