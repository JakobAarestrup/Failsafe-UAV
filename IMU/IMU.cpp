#include "IMU.hpp"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "I2C.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include "linalg.h"

// Definitions
#define PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578
#define DT 0.02       // [s/loop] loop period. 20ms
#define AA 0.97       // complementary filter constant
#define XL_Sensitivity  0.061 // [deg/LSB]
#define G_Sensitivity   70  // [deg/s/LSB]
#define MG_Sensitivity 6842
#define gyroDrift 0.07

I2C I1;

// Constructor
IMU::IMU()
{
}

// Destructor
IMU::~IMU()
{
}

/**
 * @brief Calibrates gyro from starting position
 * 
 * @param IMU handler for choosing IMU sensor.
 */
void IMU::calibrateGyro(int IMU)
{
    float gx_TOTAL, gy_TOTAL, gz_TOTAL;
    int i = 0;
    int N = 500; // amount of samples
    
    for(i = 0; i <N ; i++)
    {

        if (IMU == 1)
        {
            gx_TOTAL += I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_G,1,1);
            gy_TOTAL += I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_G,1,1);
            gz_TOTAL += I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_G,1,1);
        }
        else if (IMU == 2)
        {
            gx_TOTAL += I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G,1,1);
            gy_TOTAL += I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G,1,1);
            gz_TOTAL += I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G,1,1);
        }
        else
        {
            printf("IMU value is not supported...\n")
            break
        }

    }
    /**
     * @brief Convert 16 bit register from mdeg/s/LSB to deg/s
     * 
     */
    gx_drift = ((gx_TOTAL*G_Sensitivity)/1000)/N;
    gy_drift = ((gy_TOTAL*G_Sensitivity)/1000)/N;
    gz_drift = ((gz_TOTAL*G_Sensitivity)/1000)/N;
}


/**
 * @brief Reads data of the three sensors and subtracting offsets.
 * 
 * @param IMU handler for choosing IMU sensor.
 */

void IMU::readIMU(int IMU)
{
    readACC(IMU); 
    readGYRO(IMU);
    readMAG(IMU);
}


void IMU::readACC(int IMU)
{
    float ax,ay,az;

    /**
     * @brief handler for IMU sensor
     * 
     */
    if(IMU == 1)
    {
        ax = I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_A,1,1);
        ay = I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_A,1,1);
        az = I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_A,1,1);

    }
    else if (IMU == 2)
    {
        ax = I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A,1,1);
        ay = I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A,1,1);
        az = I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A,1,1);

    }
    else
    {
        printf("IMU value is not supported...\n")
        break
    }

    /**
     * @brief Convert 16 bit register from mg/LSB to g
     * 
     */
    ax = (ax*XL_Sensitivity)/1000;
    ay = (ay*XL_Sensitivity)/1000;
    az = (az*XL_Sensitivity)/1000;

    /**
     * @brief Calibration right here..
     * 
     */
    accCalibX_ = ax;
    accCalibY_ = ay;
    accCalibZ_ = az;

}

void IMU::readGYRO(int IMU)
{
    float gx,gy,gz;

    /**
     * @brief handler for IMU sensor
     * 
     */
    if (IMU == 1)
    {
    gx = I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_G,1,1);
    gy = I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_G,1,1);
    gz = I2C::readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_G,1,1);
    }
    else if (IMU == 2)
    {
    gx = I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G,1,1);
    gy = I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G,1,1);
    gz = I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G,1,1);
    }
    else
    {
        printf("IMU value is not supported...\n")
        break
    }


    /**
     * @brief Convert 16 bit register from mdeg/s/LSB to deg/s
     * 
     */
    gx = (gx*G_Sensitivity)/1000; 
    gy = (gy*G_Sensitivity)/1000;
    gz = (gz*G_Sensitivity)/1000;

   /**
     * @brief hardcoded offset or calibration offset
     * 
     */
    /*
    /* hardcoded offset */ 
    /*if (gx > 0) 
    {
       gyroCalibX_ = gx - gyroDrift;   // Gyroscope X-angle in deg/s
    }  

    if (gy > 0)
    {
       gyroCalibY_ = gy - gyroDrift;   // Gyroscope Y-angle in deg/s
    }   

    if (gz > 0)
    {
       gyroCalibZ_ = gz - gyroDrift;   // Gyroscope Z-angle in deg/s
    }  
    */
    /* Calibration offset */ 
    gyroCalibX_ = gx - gx_drift;   // Gyroscope X-angle in deg/s
    gyroCalibY_ = gy - gy_drift;   // Gyroscope Y-angle in deg/s
    gyroCalibZ_ = gz - gz_drift;   // Gyroscope Z-angle in deg/s 
}


void IMU::readMAG(int IMU)
{
    float mx,my,mz;
    float bx,by,bz;

    /**
     * @brief handler for IMU sensor
     * 
     */
    if(IMU == 1)
    {
        mx = I2C::readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_X_L,1,1);
        my = I2C::readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Y_L,1,1);
        mz = I2C::readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Z_L,1,1);

    }
    else if (IMU == 2)
    {
        mx = I2C::readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_X_L,1,1);
        my = I2C::readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_Y_L,1,1);
        mz = I2C::readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_Z_L,1,1);
    }
    else
    {
        printf("IMU value is not supported...\n")
        break
    }

    /**
     * @brief Convert 16 bit register from LSB/guass to uT (micro Tesla)
     * 
     */
    mx = (mx/MG_Sensitivity)*100;
    my = (my/MG_Sensitivity)*100;
    mz = (mz/MG_Sensitivity)*100;

    /**
     * @brief hardcoded offsets. b = Hard-Iron distortion and A = Soft-Iron distortion.
     * 
     */
    if(IMU = 1)
    {
        bx = 7.977849; 
        by = 3.137438; 
        bz = -5.371644;

        float A[3][3] = {{1.002979, 0.039343, -0.014713}, 
                        {0.039343, 1.019943, -0.006826}, 
                        {-0.014713, -0.006826, 1.014517}};
    }
    else if (IMU = 2) // TODO ny calibreringsdata
    {
        bx = 7.977849; 
        by = 3.137438; 
        bz = -5.371644;

        float A[3][3] = {{1.002979, 0.039343, -0.014713}, 
                        {0.039343, 1.019943, -0.006826}, 
                        {-0.014713, -0.006826, 1.014517}};
    }

    // formel magdataCalibrated = A(magdata-b)
    mx = mx - bx; 
    my = my - by;
    mz = mz - bz;

    magCalibX_ = A[0][0]*mx + A[0][1]*my + A[0][2]*mz;  // A[0,:]*(magdata-b)
    magCalibY_ = A[1][0]*mx + A[1][1]*my + A[1][2]*mz;  // A[1,:]*(magdata-b)
}

void IMU::ConvertACCData()
{   
     /*  gyroXangle_ = PI*(rate_gyr_x_ / (DT*1000));
    gyroYangle_ = PI*(rate_gyr_y_ / (DT*1000));
    gyroZangle_ = PI*(rate_gyr_z_ / (DT*1000));  */
    XL_Roll_ = atan2(accCalibX_, accCalibY_) * 180 / PI;
    XL_Pitch_ = atan2(-accCalibX_, sqrt(accCalibY_ * accCalibY_ + accCalibZ_ * accCalibZ_)) * -180 / PI;
    printf("Converted - XL_Roll: %lf, XL_pitch: %lf\n\n", XL_Roll_, XL_Pitch_);
}

void IMU::ConvertGyroData()
{ 
    double CFangleX=AA*(CFangleX+gyroCalibY_) +(1 - AA) * XL_Roll_; // 97% Gyro 3% Accelerometer
    double CFangleY=AA*(CFangleY+gyroCalibX_) +(1 - AA) * XL_Pitch_;// 97% Gyro 3% Accelerometer
    double CFangleZ=AA*(CFangleZ+gyroCalibZ_) +(1 - AA) * MAG_Yaw_; // 97% Gyro 3% Magnometer
   // printf("GyroXangle: %f, GyroYangle: %f, GyroZangle: %f\n", gyroXangle_, gyroYangle_, gyroZangle_);
    printf("Roll_filtered: %f, Pitch filtered: %f, GyroZangle: %f\n", CFangleX, CFangleY);
}

void IMU::ConvertMagData()
{	
    MAG_Yaw_ = (atan2(magCalibY_,magCalibX_) * 180 / PI); // minus magnetic_decline
    if(MAG_Yaw_ < 0) // correct yaw if under 0 degrees
    {
      MAG_Yaw_ += 360;
    //printf("magYaw: %f\n\n", MAG_Yaw_);
    }
}