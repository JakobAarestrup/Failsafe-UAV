
#ifndef EKF_H_
#define EKF_H_

#include <mbed.h>
#include "matrix.h"

class EKF
{
public:

    EKF(float);
    virtual ~EKF();
    float get_est_state(uint8_t);
    void loop(matrix *);
    void display_matrix(char);
    float getYaw(void){
        return 0;
        }
    void update(float,float,float,float,float,float);
private:
    matrix x;
    matrix x_km;
    matrix P;
    matrix Q;
    matrix R;
    matrix F;
    matrix H;
    matrix K;
    matrix E;
    float g,tau_g,m,itau_g,k1,kdm;
    float Ts;
    void dgl(matrix *);
    float my_sin(float x){return x;}
    float my_cos(float x){return 1.0f-0.5f*x*x;}
};

#endif