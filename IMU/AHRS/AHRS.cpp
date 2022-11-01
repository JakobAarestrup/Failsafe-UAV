#include "AHRS.h"

#define PI 3.141592653589793

using namespace std;

#if _BMI088 
    AHRS::AHRS(uint8_t filtertype, float TS, bool calib, I2C &i2c) : ekf(TS), Mahony_filter(TS), cf_yaw(1.0f,TS), ekf_rp(TS), ekf_rpy(TS), imu(i2c), imu2(i2c), dout1(PA_10)
#else
    AHRS::AHRS(uint8_t filtertype, float TS, bool calib, I2C &i2c) : ekf(TS), Mahony_filter(TS), cf_yaw(1.0f,TS), ekf_rp(TS), ekf_rpy(TS), imu(i2c), dout1(PA_10)
#endif
{
    /* setup storage */
    this->Ts = TS;
    this->filtertype = filtertype;
    #if _LSM9DS
        imu_setup_LSM9DS(calib);
    #else 
        imu_setup_BMX055(calib);
        #if _BMI088
            imu_setup_BMI088(calib);
        #endif
    #endif
}
// -----------------------------------------------------------------------------
void AHRS::imu_setup_LSM9DS(bool calib){
    float gyro[3], accel[3], magnet[3];
    for(uint8_t i = 0; i < 3; i++)  
        {
        gyro[i]   = 0.0f;
        accel[i]  = 0.0f;
        magnet[i] = 0.0f;
        }

    /* inform user if imu is not responsive */
    while (!imu.begin()) 
        {
        wait(1);
        printf("Failed to communicate with LSM9DS1.\r\n");
        }

    /* take mean value of N samples */
    uint8_t N = 200;
    if(calib) 
        {
        wait_ms(500);
        /* read and cumsum data */
        for(uint16_t i = 0; i < N; i++) {
            imu.readGyro();
            imu.readAccel();
            imu.readMag();
            gyro[0]   += imu.gyroX;
            gyro[1]   += imu.gyroY;
            gyro[2]   += imu.gyroZ;
            accel[0]  += imu.accX;
            accel[1]  += imu.accY;
            accel[2]  += imu.accZ;
            magnet[0] += imu.magX;
            magnet[1] += imu.magY;
            magnet[2] += imu.magZ;
            wait_ms(10);
        }
        /* calculate mean value */
        for(uint16_t i = 0; i < 3; i++) {
            gyro[i] /= (float)N;
            accel[i] /= (float)N;
            magnet[i] /= (float)N;
            }
        printf("Correct gyro: %1.5f %1.5f %1.5f accel: %1.5f %1.5f %1.5f magnet: %1.5f %1.5f %1.5f\r\n", gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], magnet[0], magnet[1], magnet[2]);
    }

    /* gyro */
    // ========== ACHTUNG HIER LSM9DS SENSOR
    raw_gx2gx.setup( 1.0, gyro[0]);
    raw_gy2gy.setup(-1.0, gyro[1]);  // y-axis reversed (lefthanded system)
    raw_gz2gz.setup( 1.0, gyro[2]);

    // pmic, 23.09.2019
    // calibration for acc and mag needs to be redone on real copter pes board with power on and spinning motors (no propellers)
    /* accel */
    raw_ax2ax.setup( 1.0f, accel[0]);  // use gain and offset here
    raw_ay2ay.setup(-1.0f, accel[1]);  // y-axis reversed // was -1,0.0
    raw_az2az.setup( 1.0,  0.0f);  // do not remove gravity!
// ========== ACHTUNG HIER LSM9DS SENSOR
    /* magnet */
    /* Caibration on 21.10.2019 QK2 delivers:
    A=[ 0.9904   0   0; 0.0605    1.0158  0;0.0397   -0.0199    0.9938]
    b0= [0.4160    0.1587   -0.0998], //see T:\T-IMS-IndNav\01_Technisches\09_PES_Board_IMU\meas_mag_aFMA\calib_magn_Okt19_aFMA.m
    */
    raw_mx2mx.setup( 0.9837f,0.252f);
    raw_my2my.setup( 1.0298f,0.1675f);
    raw_mz2mz.setup(-0.9864f,-0.1392f);     // achtung, bei gain(3) Vorzeichen drehen, b0 beibehalten!!!
//    raw_mx2mx.setup( 1.0f,0.0f);
//    raw_my2my.setup( 1.0f,0.0f);
//    raw_mz2mz.setup( 1.0f,0.0f);     // achtung, bei b0(3) Vorzeichen drehen!!!
    
    this->magnet_cal_0[0] = raw_mx2mx(magnet[0]);
    this->magnet_cal_0[1] = raw_my2my(magnet[1]);
    this->magnet_cal_0[2] = raw_mz2mz(magnet[2]);
// set EKF_RPY magnet values now:
    ekf_rpy.set_m0(this->magnet_cal_0[0],this->magnet_cal_0[1],this->magnet_cal_0[2]);
    local_time = 0.0;
    
    // the thread starts

}
//------------------------------------------------------------------------------
void AHRS::imu_setup_BMX055(bool calib){
    float gyro[3], accel[3], magnet[3];
    for(uint8_t i = 0; i < 3; i++)  
        {
        gyro[i]   = 0.0f;
        accel[i]  = 0.0f;
        magnet[i] = 0.0f;
        }

    /* take mean value of N samples */
    uint8_t N = 200;
    if(calib) 
        {
        wait_ms(500);
        /* read and cumsum data */
        for(uint16_t i = 0; i < N; i++) {
            imu.readGyro();
            imu.readAccel();
            imu.readMag();
            gyro[0]   += imu.gyroX;
            gyro[1]   += imu.gyroY;
            gyro[2]   += imu.gyroZ;
            accel[0]  += imu.accX;
            accel[1]  += imu.accY;
            accel[2]  += imu.accZ;
            magnet[0] += imu.magX;
            magnet[1] += imu.magY;
            magnet[2] += imu.magZ;
            wait_ms(5);
        }
        /* calculate mean value */
        for(uint16_t i = 0; i < 3; i++) {
            gyro[i] /= (float)N;
            accel[i] /= (float)N;
            magnet[i] /= (float)N;
            }
    }
// ========== ACHTUNG HIER BMX055 SENSOR
    /* gyro */
    raw_gx2gx.setup( 1.0, gyro[0]);
    raw_gy2gy.setup( 1.0, gyro[1]);  // y-axis reversed (lefthanded system)
    raw_gz2gz.setup( 1.0, gyro[2]);

    /* accel */
    raw_ax2ax.setup( 1.0f, accel[0]);  // use gain and offset here
    raw_ay2ay.setup( 1.0f, accel[1]);  // 
    raw_az2az.setup( 1.0,  0.0f);  // do not remove gravity!

// ========== ACHTUNG HIER BMX055 SENSOR
    /* magnet */
    // diag(A0) =  0.8047    0.8447    0.8474
    // diag(A0) = 0.7485    0.8074    0.8892    //10.1.2020
    // b0       = -0.1195    0.2009   -0.3133, calibration at desk 19.12.2019
    // b0       = 0.2656    0.4564    0.9155    // 10.1.20
    //  1.0150    1.0843    1.1814, 0.2826    0.4353    0.9392   aFMA 10.1.2020
    raw_mx2mx.setup(1.0150f,0.2826f);
    raw_my2my.setup(1.0843f,0.4353f);
    raw_mz2mz.setup(1.1814f,0.9392f);      // */
/*    raw_mx2mx.setup( 1.0f,0.0f);
    raw_my2my.setup( 1.0f,0.0f);
    raw_mz2mz.setup( 1.0f,0.0f); //        */
    this->magnet_cal_0[0] = raw_mx2mx(magnet[0]);
    this->magnet_cal_0[1] = raw_my2my(magnet[1]);
    this->magnet_cal_0[2] = raw_mz2mz(magnet[2]);
// set EKF_RPY magnet values now:
    ekf_rpy.set_m0(this->magnet_cal_0[0],this->magnet_cal_0[1],this->magnet_cal_0[2]);
    local_time = 0.0;
    printf("initialized BMX055\r\n");
    
    // the thread starts

}
//------------------------------------------------------------------------------
#if _BMI088 
void AHRS::imu_setup_BMI088(bool calib){
    float gyro[3], accel[3];
    for(uint8_t i = 0; i < 3; i++)  
        {
        gyro[i]   = 0.0f;
        accel[i]  = 0.0f;
        }

    /* take mean value of N samples */
    uint8_t N = 200;
    if(calib) 
        {
        wait_ms(500);
        /* read and cumsum data */
        for(uint16_t i = 0; i < N; i++) {
            imu2.readGyro();
            imu2.readAccel();
            gyro[0]   += imu2.gyroX;
            gyro[1]   += imu2.gyroY;
            gyro[2]   += imu2.gyroZ;
            accel[0]  += imu2.accX;
            accel[1]  += imu2.accY;
            accel[2]  += imu2.accZ;
            wait_ms(5);
        }
        /* calculate mean value */
        for(uint16_t i = 0; i < 3; i++) {
            gyro[i] /= (float)N;
            accel[i] /= (float)N;
            }
    }
// ========== ACHTUNG HIER BMI088 SENSOR
    /* gyro */
    raw_gx2gx88.setup( 1.0, gyro[0]);
    raw_gy2gy88.setup( 1.0, gyro[1]);  // 
    raw_gz2gz88.setup( 1.0, gyro[2]);

    /* accel */
    raw_ax2ax88.setup( 1.0f, accel[0]);  // use gain and offset here
    raw_ay2ay88.setup( 1.0f, accel[1]);  // 
    raw_az2az88.setup( 1.0,  0.0f);  // do not remove gravity!
    printf("initialized BMI088\r\n");
}
#endif

AHRS::~AHRS() {}

void AHRS::update(void)
{   
    //
    switch(filtertype){
        case 1:
            ekf.update(data.sens_gyr[0],data.sens_gyr[1],data.sens_gyr[2],data.sens_acc[0],data.sens_acc[1],data.sens_acc[2]);
            cf_yaw.update(data.sens_gyr[2],data.sens_mag[0],data.sens_mag[1],data.sens_mag[2]);
            data.est_RPY[0] = ekf.get_est_state(0);
            data.est_RPY[1] = ekf.get_est_state(1);
            //data.est_Vxyz[0] = ekf.get_est_state(4);
            //data.est_Vxyz[1] = ekf.get_est_state(5);
            break;
        case 2:
            Mahony_filter.update(data.sens_gyr[0],data.sens_gyr[1],data.sens_gyr[2],data.sens_acc[0],data.sens_acc[1],data.sens_acc[2],0.0f,0.0f,0.0f);
            data.est_RPY[0] = Mahony_filter.getRollRadians();
            data.est_RPY[1] = Mahony_filter.getPitchRadians();
            break;
        case 3:
            ekf_rp.update(data.sens_gyr[0],data.sens_gyr[1],data.sens_acc[0],data.sens_acc[1]);

            cf_yaw.update(data.sens_gyr[2],data.sens_mag[0],data.sens_mag[1],data.sens_mag[2]);
            data.est_RPY[0] = ekf_rp.get_est_state(0);
            data.est_RPY[1] = ekf_rp.get_est_state(1);
            break;
        case 4:
            ekf_rpy.update(data.sens_gyr[0],data.sens_gyr[1],data.sens_gyr[2],data.sens_acc[0],data.sens_acc[1],data.sens_mag[0],data.sens_mag[1]);
            data.est_RPY[0] = ekf_rpy.get_est_state(0);
            data.est_RPY[1] = ekf_rpy.get_est_state(1);
            data.est_RPY[2] = ekf_rpy.get_est_state(2);
            data.est_RPY[3] = ekf_rpy.get_est_state(3);
            data.est_RPY[4] = ekf_rpy.get_est_state(4);
            data.est_RPY[5] = ekf_rpy.get_est_state(5);
            data.est_RPY[6] = ekf_rpy.get_est_state(6);
            data.est_RPY[7] = ekf_rpy.get_est_state(7);
            break;
        }
 //   dout3.write(0);
}
void AHRS::update_as_thread(void)
{   
    while(1)
        {
        thread.signal_wait(signal);
//        dout3.write(1);
        if(filtertype !=1)
            {
            Mahony_filter.update(data.sens_gyr[0],data.sens_gyr[1],data.sens_gyr[2],data.sens_acc[0],data.sens_acc[1],data.sens_acc[2],0.0f,0.0f,0.0f);
            data.est_RPY[0] = Mahony_filter.getRollRadians();
            data.est_RPY[1] = Mahony_filter.getPitchRadians();
            }
        else
            {
            ekf.update(data.sens_gyr[0],data.sens_gyr[1],data.sens_gyr[2],data.sens_acc[0],data.sens_acc[1],data.sens_acc[2]);
            data.est_RPY[0] = ekf.get_est_state(0);
            data.est_RPY[1] = ekf.get_est_state(1);
            }
//            dout3.write(0);
        }
}
void AHRS::read_imu_sensors(void){
//    mutex.lock();
dout1.write(1);
    imu.readGyro();
    imu.readAccel();
    imu.readMag();
dout1.write(0);
    data.sens_gyr[0] = raw_gx2gx(imu.gyroX);
    data.sens_gyr[1] = raw_gy2gy(imu.gyroY);
    data.sens_gyr[2] = raw_gz2gz(imu.gyroZ);
    data.sens_acc[0] = raw_ax2ax(imu.accX);
    data.sens_acc[1] = raw_ay2ay(imu.accY);
    data.sens_acc[2] = raw_az2az(imu.accZ);
    data.sens_mag[0] = raw_mx2mx(imu.magX);
    data.sens_mag[1] = raw_my2my(imu.magY);
    data.sens_mag[2] = raw_mz2mz(imu.magZ);
//    mutex.unlock();
#if _BMI088 
    imu2.readGyro();
    imu2.readAccel();
    data.sens_gyr[3] = raw_gx2gx88(imu2.gyroX);
    data.sens_gyr[4] = raw_gy2gy88(imu2.gyroY);
    data.sens_gyr[5] = raw_gz2gz88(imu2.gyroZ);
    data.sens_acc[3] = raw_ax2ax88(imu2.accX);
    data.sens_acc[4] = raw_ay2ay88(imu2.accY);
    data.sens_acc[5] = raw_az2az88(imu2.accZ);
#endif
}
// ------------------- start controllers ----------------
void AHRS::start_loop(void){
    thread.start(callback(this, &AHRS::update_as_thread));
    ticker.attach(callback(this, &AHRS::sendSignal), Ts);
}

// this is for realtime OS
void AHRS::sendSignal() {
    thread.signal_set(signal);
}