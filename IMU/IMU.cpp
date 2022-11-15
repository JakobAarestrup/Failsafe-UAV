#include "IMU.hpp"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "I2C.hpp"

// Definitions
#define PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578
#define DT 0.02       // [s/loop] loop period. 20ms
#define AA 0.97       // complementary filter constant
#define XL_Sensitivity 0.061 // [deg/LSB]
#define G_GAIN 0.07  // [deg/s/LSB]

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

    if(aX > 0x8000) 
    {
        aX = aX - 0xFFFF;
    }

    if (aY > 0x8000)
    {
        aY = aY - 0xFFFF;
    }
    
    if (aZ > 0x8000)
    {
        aZ = aZ - 0xFFFF; // lave det her i I2C read kald?
    } 
 
    printf("after if.. - X: %f, Y: %f Z: %f\n", aX, aY, aZ);

    float aXf = (aX*0.061)/1000;
    float aYf = (aY*0.061)/1000;
    float aZf = (aZ*0.061)/1000;

    XL_Roll_ = atan2(aYf, aZf) * 180 / PI;
    XL_Pitch_ = atan2(-aXf, sqrt(aYf * aYf + aZf * aZf)) * 180 / PI;
    printf("Converted - XL_Roll: %lf, XL_pitch: %lf\n\n", XL_Roll_, XL_Pitch_);
}

void IMU::ConvertGyroData(float gX, float gY, float gZ)
{
    printf("RawgX: %f, RawgY: %f, RawgZ: %f\n", gX, gY, gZ);
    rate_gyr_x_ = (gX/70)/1000;
    rate_gyr_y_ = (gY/70)/1000;
    rate_gyr_z_ = (gZ/70)/1000;
    printf("GyroX: %f, GyroY: %f, GyroZ: %f\n", rate_gyr_x_, rate_gyr_y_, rate_gyr_z_);
    gyroXangle_ += rate_gyr_x_ * DT;
    gyroYangle_ += rate_gyr_y_ * DT;
    gyroZangle_ += rate_gyr_z_ * DT;
    printf("GyroXangle: %f, GyroYangle: %f, GyroZangle: %f\n", gyroXangle_, gyroYangle_, gyroZangle_);
    double CFangleX=AA*(CFangleX+rate_gyr_x_*DT) +(1 - AA) * XL_Roll_;
    double CFangleY=AA*(CFangleY+rate_gyr_y_*DT) +(1 - AA) * XL_Pitch_;
    printf("Roll_filtered: %f, Pitch filtered: %f, GyroZangle: %f\n", CFangleX, CFangleY);
}

void IMU::ConvertMagData(float mY, float mX)
{	
    //float mg_variation = 217.9 / 1000.0; få Magnetic variation i millirad
    magYaw_ = 180 * (atan2(mY,mX)/PI);
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
    // set offset variabel
    // anden kalibrering
}