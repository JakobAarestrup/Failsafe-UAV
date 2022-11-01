#include "EKF.h"

using namespace std;

EKF::EKF(float Ts) : x(6,1,0.0),x_km(6,1,0.0),P(6,0.001),Q(6),R(2,0.00045),F(6,6,0.0),H(2,6,0.0),K(6,2,0.0),E(6)
{
    
    g = -9.81;  // -1 wege Drehung des z-Vektors (z nach oben)
    tau_g = 2.0;
    itau_g = 1.0f/tau_g;
    k1 = 0.55;
    m = 1.1;
    kdm = k1/m;
    this->Ts = Ts;
    
    F.put_entry(0,2,-1);
    F.put_entry(1,3,-1);
    F.put_entry(2,2,-1/tau_g);
    F.put_entry(3,3,-1/tau_g);
    F.put_entry(4,1,-g);
    F.put_entry(5,0,g);
    F.put_entry(4,4,-k1/m);
    F.put_entry(5,5,-k1/m); // F is continuous Linearized!!
    H.put_entry(0,4,-kdm);
    H.put_entry(1,5,-kdm);
    F.scale(Ts);
    F.mcopy(&(F+E));
    Q.mcopy(&(F/=F));   // Q=A*A'
    Q.scale(1e-11);
    //H.printout();
    //P.printout();
    }
    
    
EKF::~EKF(void) {
    printf("EKF is being deleted\r\n");
    }

void EKF::dgl(matrix *Z)
{
    
    x_km.a[0][0]= x.a[0][0]+Ts*(Z->a[0][0]-x.a[2][0]+tan(x.a[1][0])*my_sin(x.a[0][0])*(Z->a[1][0]-x.a[3][0]));
    x_km.a[1][0]= x.a[1][0]+Ts*(my_cos(x.a[0][0])*(Z->a[1][0]-x.a[3][0]));
    x_km.a[2][0]= x.a[2][0]+Ts*(-itau_g*x.a[2][0]);
    x_km.a[3][0]= x.a[3][0]+Ts*(-itau_g*x.a[3][0]);
    x_km.a[4][0]= x.a[4][0]+Ts*(-g*my_sin(x.a[1][0])-kdm*x.a[4][0]);
    x_km.a[5][0]= x.a[5][0]+Ts*(g*my_cos(x.a[1][0])*my_sin(x.a[0][0])-kdm*x.a[5][0]);
    }
// Just map gx ... to measurement matrix (to be consistent with Mahony etc.
void EKF::update(float gx,float gy,float gz,float ax,float ay,float az){
    matrix measurement(6, 1, 0.0f);
    measurement.put_entry(0, 0, gx);
    measurement.put_entry(1, 0, gy);
    measurement.put_entry(3, 0, ax);
    measurement.put_entry(4, 0, ay);
    loop(&measurement);
    }  
// 
void EKF::loop(matrix *Z)
{
    // F_km1 is constant

    dgl(Z);                          // here x_km is calculated
    P.mcopy(&(((F*P)/=F)+Q));                        //P_km = F_km1*P_km*F_km1'+Q_k

    matrix dum1(2);
    dum1.mcopy(&(((H*P)/=H)+R));
    K.mcopy(&((P/=H)*dum1.inv_2x2()));  // K_k=(P_km*H_k')*inv(H_k*P_km*H_k'+R_k); % Kalman-Gain
                                        // x_k = x_km + K_k*(Z(r,[4 5])'-copter3D_calc_h_ekf(x_km,kin)); 
    matrix dum2(2,1,0.0);
    dum2.put_entry(0,0,Z->a[3][0]+kdm*x_km.a[4][0]);
    dum2.put_entry(1,0,Z->a[4][0]+kdm*x_km.a[5][0]);     // calculates Z-h h is -k/m*...
    x_km += (K*dum2);
    x.mcopy(&x_km);
                                        // P_km = (eye(6)-K_k*H_k)*P_km; % Kovar
    P.mcopy(&((E-(K*H))*P));
  
    }
float EKF::get_est_state(uint8_t i)
{
    return x_km.a[i][0];
    }