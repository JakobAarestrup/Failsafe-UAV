#include "IMU.hpp"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "I2C.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"

// Definitions
#define PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578
#define DT 0.02       // [s/loop] loop period. 20ms
#define AA 0.97       // complementary filter constant
#define XL_Sensitivity 0.061 // [deg/LSB]
#define G_GAIN 0.07  // [deg/s/LSB]

I2C I1;

// Constructor
IMU::IMU()
{
}

// Destructor
IMU::~IMU()
{
}

void IMU::ConvertACCData(float aX, float aY, float aZ)
{
    printf("non-Converted - X: %f, Y: %f Z: %f\n", aX, aY, aZ);

    float aXf = (aX*0.061)/1000;
    float aYf = (aY*0.061)/1000;
    float aZf = (aZ*0.061)/1000;

    XL_Roll_ = atan2(aYf, aZf) * 180 / PI;
    XL_Pitch_ = atan2(-aXf, sqrt(aYf * aYf + aZf * aZf)) * -180 / PI;
    printf("Converted - XL_Roll: %lf, XL_pitch: %lf\n\n", XL_Roll_, XL_Pitch_);
}

void IMU::ConvertGyroData(float gX, float gY, float gZ)
{
    
    printf("RawgX: %f, RawgY: %f, RawgZ: %f\n", gX, gY, gZ);
    rate_gyr_x_ = (gX*17.5)/1000; // gyro_sensitivity
    rate_gyr_y_ = (gY*17.5)/1000; // gyro_sensitivity 
    rate_gyr_z_ = (gZ*17.5)/1000; // gyro_sensitivity
    printf("GyroX: %f, GyroY: %f, GyroZ: %f\n", rate_gyr_x_, rate_gyr_y_, rate_gyr_z_);
    gyroXangle_ = PI*(rate_gyr_x_ / (DT*1000));
    gyroYangle_ = PI*(rate_gyr_y_ / (DT*1000));
    gyroZangle_ = PI*(rate_gyr_z_ / (DT*1000)); 
   
    double CFangleX=AA*(CFangleX+gyroYangle_) +(1 - AA) * XL_Roll_; // 97% Gyro 3% Accelerometer
    double CFangleY=AA*(CFangleY+gyroXangle_) +(1 - AA) * XL_Pitch_;// 97% Gyro 3% Accelerometer
     printf("GyroXangle: %f, GyroYangle: %f, GyroZangle: %f\n", gyroXangle_, gyroYangle_, gyroZangle_);
    printf("Roll_filtered: %f, Pitch filtered: %f, GyroZangle: %f\n", CFangleX, CFangleY);
}

void IMU::ConvertMagData(float mY, float mX)
{	
    //float mg_variation = 217.9 / 1000.0; få Magnetic variation i millirad
    magYaw_ = 180 * (atan2(mY,mX)/PI); // minus magnetic_decline
    // magYaw_ += mg_variation * 180/PI;
    if(magYaw_ < 0) // correct yaw if under 0
      magYaw_ += 360;
    printf("magYaw: %f\n\n", magYaw_);
}

void IMU::calibrateIMU()
{
    // få data fra acc
    //I2C::readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A,1,1);

    // set offset variabel
    // anden kalibrering

    // få data fra gyr
    // set offset variabel
    // anden kalibrering

    // få data fra mag
    float mx = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_X_L,1,1);
    float my = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_Y_L,1,1);
    float mz = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_Z_L,1,1);

    float A[3][3] = {1.002979, 0.039343, -0.014713, 0.039343, 1.019943, -0.006826, -0.014713, -0.006826, 1.014517};
    float b[3] = {7.977849, 3.137438, -5.371644};

    
    
}