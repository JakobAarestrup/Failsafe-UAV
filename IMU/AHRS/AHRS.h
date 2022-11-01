#ifndef AHRS_H_
#define AHRS_H_


#include "Mahony.h"
#include "MadgwickAHRS.h"
#include "LinearCharacteristics.h"
#include "LSM9DS1_i2c.h"
#include "BMX055.h"
#include "BMI088.h"

#include "Signal.h"
#include "EKF.h"
#include "EKF_RP.h"
#include "EKF_RPY.h"
#include "CF_Yaw.h"
#include "matrix.h"
#include "Data_Logger.h"
#include "Read_Xtern_Sensors.h"

extern DATA_Xchange data;

class AHRS
{
public:

    AHRS(uint8_t,float,bool,I2C &);
    
    virtual ~AHRS();

    float getRoll(uint8_t ft)
    {   
        switch(ft){
            case 1:
                return ekf.get_est_state(0);
            case 2:
                return Mahony_filter.getRollRadians();
            case 3:
                return ekf_rp.get_est_state(0);
            case 4:
                return ekf_rpy.get_est_state(0);
            default:
            return 0.0;
        }
    }
    float getPitch(uint8_t ft)
        {   
        switch (ft){
            case 1:
                return ekf.get_est_state(1);
            case 2:
                return Mahony_filter.getPitchRadians();
            case 3:
                return ekf_rp.get_est_state(1);
            case 4:
                return ekf_rpy.get_est_state(1);
            default:
            return 0.0;
        }
    }
    float getYaw(uint8_t ft)
        {   
        switch (ft){
            case 4:
                return ekf_rpy.get_est_state(2);
            default:
                return 0.0;
        }
    }
    #if _LSM9DS
        LSM9DS1 imu;
    #else
        BMX055 imu;
        #if _BMI088    
            BMI088 imu2;
        #endif
    #endif
    Mahony Mahony_filter;
    EKF ekf;
    EKF_RP ekf_rp;
    EKF_RPY ekf_rpy;
    CF_Yaw cf_yaw;
  
    LinearCharacteristics raw_gx2gx;
    LinearCharacteristics raw_gy2gy;
    LinearCharacteristics raw_gz2gz;
    LinearCharacteristics raw_ax2ax;
    LinearCharacteristics raw_ay2ay;
    LinearCharacteristics raw_az2az;
    LinearCharacteristics raw_gx2gx88;
    LinearCharacteristics raw_gy2gy88;
    LinearCharacteristics raw_gz2gz88;
    LinearCharacteristics raw_ax2ax88;
    LinearCharacteristics raw_ay2ay88;
    LinearCharacteristics raw_az2az88;
    LinearCharacteristics raw_mx2mx;
    LinearCharacteristics raw_my2my;
    LinearCharacteristics raw_mz2mz;
    void read_imu_sensors(void);
    float xyzUS[3];
    float xyzAS[3];
    float rxryrzUS[3];
    float v_xyzOF[2];
    float xyzOF[3];
    float xyzLIDAR[3];
    float xyzRS[3];
    float magnet_cal_0[3];
    void update();
    void update_as_thread(void);
    void start_loop(void);
private:
    float local_time;
    uint8_t filtertype;
    Signal signal;
    Thread thread;
    Ticker ticker;
    void sendSignal();
    void imu_setup_LSM9DS(bool calib);
    void imu_setup_BMX055(bool calib);
    void imu_setup_BMI088(bool calib);
    float Ts;
    DigitalOut dout1;
    Mutex mutex;
};

#endif