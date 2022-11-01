#include "EKF_RPY.h"

using namespace std;
using namespace Eigen;

EKF_RPY::EKF_RPY(float Ts, float mx0, float my0, float mz0)
{
    this->Ts = Ts;
    set_para();
    m0 << mx0, my0, mz0;
    reset();
}

EKF_RPY::EKF_RPY(float Ts) 
{
    this->Ts = Ts;
    set_para();
    reset();
}

EKF_RPY::~EKF_RPY() {}

void EKF_RPY::set_para()
{   
    /* [n_gyro; n_v; n_b_g] */
    var_fx << 0.1f, 0.1f, 0.1f, 0.002f, 0.002f, 0.002f, 0.002f, 0.002f;
    /* [n_acc; n_mag] */
    var_gy << 40.0f, 40.0f, 1.0f, 1.0f;
    rho = 1.0f;
    // kv = 0.5f;       /* QK3 k1/m */
    kv = 0.26f;       /* QK4 k1/m */
    g = 9.81f;
    scale_P0 = 1000.0f;
}

void EKF_RPY::reset()
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

void EKF_RPY::increase_diag_P()
{
    P = P + scale_P0 * I;
}

void EKF_RPY::set_m0(float mx0, float my0, float mz0)
{
    m0 << mx0, my0, mz0;
}

float EKF_RPY::get_est_state(uint8_t i)
{   
    /* x = [ang; v; b_g] = [0: phi
                            1: theta
                            2: psi
                            3: b_gx
                            4: b_gy
                            5: b_gz
                            6: vx
                            7: vy] */
    return x(i);
}

void EKF_RPY::update(float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float magnet_x, float magnet_y)
{
    u << gyro_x, gyro_y, gyro_z;
    y << accel_x, accel_y, magnet_x, magnet_y;
    update_angles();
    
    calc_F();
    calc_H();
    calc_Q();
    
    x = fxd();
    P = F * P * F.transpose() + Q;
    e = y - gy();
    
    /* inversion faster 356 mus < 432 mus recursion */
    K = P * H.transpose() * ( H * P * H.transpose() + R ).inverse();
    x = x + K * e;
    P = (I - K * H) * P;
    
    /* only valid if R is diagonal (uncorrelated noise) */
    /*
    for(uint8_t i = 0; i < 4; i++)  {
        K.col(i) = ( P * (H.row(i)).transpose() ) / ( H.row(i) * P * (H.row(i)).transpose() + R(i,i) );
        x = x + K.col(i) * e(i);
        P = (I - K.col(i)*H.row(i)) * P;
    }
    */
}

void EKF_RPY::update_angles()
{
    s1 = sinf(x(0));
    c1 = cosf(x(0));
    s2 = sinf(x(1));
    c2 = cosf(x(1));
    s3 = sinf(x(2));
    c3 = cosf(x(2));   
}

void EKF_RPY::calc_F()
{
    F << Ts*(c1*s2*(u(1) - x(6)) - s1*s2*(u(2) - x(7)))/c2 + 1.0f,    Ts*(c1*(u(2) - x(7)) + s1*(u(1) - x(6)))/(c2*c2), 0.0f,              0.0f,             0.0f,  -Ts, -Ts*s1*s2/c2, -Ts*c1*s2/c2,
                        -Ts*(c1*(u(2) - x(7)) + s1*(u(1) - x(6))),                                                1.0f, 0.0f,              0.0f,             0.0f, 0.0f,       -Ts*c1,        Ts*s1,
                      Ts*(c1*(u(1) - x(6)) - s1*(u(2) - x(7)))/c2, Ts*(c1*(u(2) - x(7)) + s1*(u(1) - x(6)))*s2/(c2*c2), 1.0f,              0.0f,             0.0f, 0.0f,    -Ts*s1/c2,    -Ts*c1/c2,
                                                             0.0f,                                             Ts*c2*g, 0.0f,      1.0f - Ts*kv, Ts*(u(2) - x(7)), 0.0f,         0.0f,     -Ts*x(4),
                                                      -Ts*c1*c2*g,                                          Ts*g*s1*s2, 0.0f, -Ts*(u(2) - x(7)),     1.0f - Ts*kv, 0.0f,         0.0f,      Ts*x(3),
                                                             0.0f,                                                0.0f, 0.0f,              0.0f,             0.0f, 1.0f,         0.0f,         0.0f,
                                                             0.0f,                                                0.0f, 0.0f,              0.0f,             0.0f, 0.0f,         1.0f,         0.0f,
                                                             0.0f,                                                0.0f, 0.0f,              0.0f,             0.0f, 0.0f,         0.0f,         1.0f;
}

void EKF_RPY::calc_H()
{
    H <<                                                              0.0f,                                      0.0f,                                                  0.0f,         -kv, u(2) - x(7), 0.0f, 0.0f, -x(4),
                                                                      0.0f,                                      0.0f,                                                  0.0f, x(7) - u(2),         -kv, 0.0f, 0.0f,  x(3),
                                                                      0.0f,    - c2*m0(2) - c3*m0(0)*s2 - m0(1)*s2*s3,                              c2*(c3*m0(1) - m0(0)*s3),        0.0f,        0.0f, 0.0f, 0.0f,  0.0f,
         m0(0)*(s1*s3 + c1*c3*s2) - m0(1)*(c3*s1 - c1*s2*s3) + c1*c2*m0(2), s1*(c2*c3*m0(0) - m0(2)*s2 + c2*m0(1)*s3), - m0(0)*(c1*c3 + s1*s2*s3) - m0(1)*(c1*s3 - c3*s1*s2),        0.0f,        0.0f, 0.0f, 0.0f,  0.0f;
}

void EKF_RPY::initialize_R()
{
    R << rho*var_gy(0)/Ts,             0.0f,             0.0f,             0.0f,
                     0.0f, rho*var_gy(1)/Ts,             0.0f,             0.0f,
                     0.0f,             0.0f, rho*var_gy(2)/Ts,             0.0f,
                     0.0f,             0.0f,             0.0f, rho*var_gy(3)/Ts;
}

void EKF_RPY::initialize_Q()
{
    Q << Ts*(var_fx(0) + (c1*c1*var_fx(2) + s1*s1*var_fx(1))*s2*s2/(c2*c2)), Ts*(var_fx(1) - var_fx(2))*c1*s1*s2/c2, Ts*(c1*c1*var_fx(2) + s1*s1*var_fx(1))*s2/(c2*c2),         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
                                     Ts*(var_fx(1) - var_fx(2))*c1*s1*s2/c2, Ts*(var_fx(1)*c1*c1 + var_fx(2)*s1*s1),               Ts*(var_fx(1) - var_fx(2))*c1*s1/c2,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
                          Ts*(c1*c1*var_fx(2) + s1*s1*var_fx(1))*s2/(c2*c2),    Ts*(var_fx(1) - var_fx(2))*c1*s1/c2,    Ts*(c1*c1*var_fx(2) + s1*s1*var_fx(1))/(c2*c2),         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
                                                                       0.0f,                                   0.0f,                                              0.0f, Ts*var_fx(3),         0.0f,         0.0f,         0.0f,         0.0f,
                                                                       0.0f,                                   0.0f,                                              0.0f,         0.0f, Ts*var_fx(4),         0.0f,         0.0f,         0.0f,
                                                                       0.0f,                                   0.0f,                                              0.0f,         0.0f,         0.0f, Ts*var_fx(5),         0.0f,         0.0f,
                                                                       0.0f,                                   0.0f,                                              0.0f,         0.0f,         0.0f,         0.0f, Ts*var_fx(6),         0.0f,
                                                                       0.0f,                                   0.0f,                                              0.0f,         0.0f,         0.0f,         0.0f,         0.0f, Ts*var_fx(7);
}

void EKF_RPY::calc_Q()
{
    Q(0,0) = Ts*(var_fx(0) + (c1*c1*var_fx(2) + s1*s1*var_fx(1))*s2*s2/(c2*c2));
    Q(0,1) = Ts*(var_fx(1) - var_fx(2))*c1*s1*s2/c2;
    Q(0,2) = Ts*(c1*c1*var_fx(2) + s1*s1*var_fx(1))*s2/(c2*c2);
    Q(1,0) = Q(0,1);
    Q(1,1) = Ts*(var_fx(1)*c1*c1 + var_fx(2)*s1*s1);
    Q(1,2) = Ts*(var_fx(1) - var_fx(2))*c1*s1/c2;
    Q(2,0) = Q(0,2);
    Q(2,1) = Q(1,2);
    Q(2,2) = Ts*(c1*c1*var_fx(2) + s1*s1*var_fx(1))/(c2*c2);
}

Matrix <float, 8, 1>  EKF_RPY::fxd()
{
    Matrix <float, 8, 1> retval;
    retval <<  x(0) + Ts*(u(0) - x(5) + (c1*s2*(u(2) - x(7)))/c2 + (s1*s2*(u(1) - x(6)))/c2),
                                             x(1) + Ts*(c1*(u(1) - x(6)) - s1*(u(2) - x(7))),
                                   x(2) + Ts*((c1*(u(2) - x(7)))/c2 + (s1*(u(1) - x(6)))/c2),
                                             x(3) + Ts*(g*s2 - kv*x(3) + x(4)*(u(2) - x(7))),
                                          x(4) - Ts*(kv*x(4) + x(3)*(u(2) - x(7)) + c2*g*s1),
                                                                                        x(5),
                                                                                        x(6),
                                                                                        x(7);
    return retval;
}

Matrix <float, 4, 1> EKF_RPY::gy()
{
    Matrix <float, 4, 1> retval;
    retval <<                                         x(4)*(u(2) - x(7)) - kv*x(3),
                                                    - kv*x(4) - x(3)*(u(2) - x(7)),
                                            - m0(2)*s2 + c2*c3*m0(0) + c2*m0(1)*s3,
               - m0(0)*(c1*s3 - c3*s1*s2) + m0(1)*(c1*c3 + s1*s2*s3) + c2*m0(2)*s1;
    return retval;
}

