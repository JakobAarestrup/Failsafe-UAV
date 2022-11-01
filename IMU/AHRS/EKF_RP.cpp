#include "EKF_RP.h"

using namespace std;
using namespace Eigen;

EKF_RP::EKF_RP(float Ts)
{
    this->Ts = Ts;
    /* [n_gyro; n_b_g; n_v] */
    var_fx << 0.1f, 0.1f, 0.002f, 0.002f, 0.002f, 0.002f;
    /* [n_acc] */
    var_gy << 40.0f, 40.0f;
    rho = 1.0f;
    // kv = 0.5f;       /* QK3 k1/m */
    kv = 0.26f;       /* QK4 k1/m */
    g = 9.81f;
    scale_P0 = 1000.0f;
    reset();
}

EKF_RP::~EKF_RP() {}

void EKF_RP::reset()
{   
    u.setZero();
    y.setZero();
    x.setZero();
    update_angles();
    calc_F();
    calc_H();
    initialize_Q();
    initialize_R();
    K.setZero();
    I.setIdentity();
    e.setZero();
    P = scale_P0 * I;
}

void EKF_RP::increase_diag_P()
{
    P = P + scale_P0 * I;      
}

float EKF_RP::get_est_state(uint8_t i)
{   
    /* x = [ang; v; b_g] = [0: phi
                            1: theta
                            2: vx
                            3: vy
                            4: b_gx
                            5: b_gy] */
    return x(i);
}

void EKF_RP::update(float gyro_x, float gyro_y, float accel_x, float accel_y)
{
    u << gyro_x, gyro_y;
    y << accel_x, accel_y;
    update_angles();
    
    calc_F();
    // calc_H(); /* H remains constant */
    calc_Q();
    
    x = fxd();
    P = F * P * F.transpose() + Q;
    e = y - gy();
    
    /* inversion faster 184 mus < 207 mus recursion */
    K = P * H.transpose() * ( H * P * H.transpose() + R ).inverse();
    x = x + K * e;
    P = (I - K * H) * P;
    
    /* only valid if R is diagonal (uncorrelated noise) */
    /*
    for(uint8_t i = 0; i < 2; i++)  {
        K.col(i) = ( P * (H.row(i)).transpose() ) / ( H.row(i) * P * (H.row(i)).transpose() + R(i,i) );
        x = x + K.col(i) * e(i);
        P = (I - K.col(i)*H.row(i)) * P;
    }
    */
}

void EKF_RP::update_angles()
{
    s1 = sinf(x(0));
    c1 = cosf(x(0));
    s2 = sinf(x(1));
    c2 = cosf(x(1));   
}

void EKF_RP::calc_F()
{
    F << Ts*c1*s2*(u(1) - x(3))/c2 + 1.0f, Ts*s1*(u(1) - x(3))/(c2*c2),  -Ts, -Ts*s1*s2/c2,         0.0f,         0.0f,
                     -Ts*s1*(u(1) - x(3)),                        1.0f, 0.0f,       -Ts*c1,         0.0f,         0.0f,
                                     0.0f,                        0.0f, 1.0f,         0.0f,         0.0f,         0.0f,
                                     0.0f,                        0.0f, 0.0f,         1.0f,         0.0f,         0.0f,
                                     0.0f,                     Ts*c2*g, 0.0f,         0.0f, 1.0f - Ts*kv,         0.0f,
                              -Ts*c1*c2*g,                  Ts*g*s1*s2, 0.0f,         0.0f,         0.0f, 1.0f - Ts*kv;
}

void EKF_RP::calc_H()
{
    H << 0.0f, 0.0f, 0.0f, 0.0f,  -kv, 0.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  -kv;
}

void EKF_RP::initialize_R()
{
    R <<  rho*var_gy(0)/Ts,             0.0f,
                      0.0f, rho*var_gy(1)/Ts;
}

void EKF_RP::initialize_Q()
{   
    Q << Ts*(var_fx(0) + s1*s1*s2*s2*var_fx(1)/(c2*c2)), Ts*c1*s1*s2*var_fx(1)/c2,         0.0f,         0.0f,         0.0f,         0.0f,
                               Ts*c1*s1*s2*var_fx(1)/c2,       Ts*c1*c1*var_fx(1),         0.0f,         0.0f,         0.0f,         0.0f,
                                                   0.0f,                     0.0f, Ts*var_fx(2),         0.0f,         0.0f,         0.0f,
                                                   0.0f,                     0.0f,         0.0f, Ts*var_fx(3),         0.0f,         0.0f,
                                                   0.0f,                     0.0f,         0.0f,         0.0f, Ts*var_fx(4),         0.0f,
                                                   0.0f,                     0.0f,         0.0f,         0.0f,         0.0f, Ts*var_fx(5);
}

void EKF_RP::calc_Q()
{   
    Q(0,0) = Ts*(var_fx(0) + s1*s1*s2*s2*var_fx(1)/(c2*c2));
    Q(0,1) = Ts*c1*s1*s2*var_fx(1)/c2;
    Q(1,0) = Q(0,1);
    Q(1,1) = Ts*c1*c1*var_fx(1);
}

Matrix <float, 6, 1>  EKF_RP::fxd()
{
    Matrix <float, 6, 1> retval;
    retval << x(0) + Ts*(u(0) - x(2) + (s1*s2*(u(1) - x(3)))/c2),
                                      x(1) + Ts*c1*(u(1) - x(3)),
                                                            x(2),
                                                            x(3),
                                      x(4) + Ts*(g*s2 - kv*x(4)),
                                   x(5) - Ts*(kv*x(5) + c2*g*s1);
    return retval;
}

Matrix <float, 2, 1> EKF_RP::gy()
{
    Matrix <float, 2, 1> retval;
    retval << -kv*x(4),
              -kv*x(5);
    return retval;
}

