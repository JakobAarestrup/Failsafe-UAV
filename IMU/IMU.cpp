#include "IMU.hpp"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Definitions
#define PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578
#define DT 0.02       // [s/loop] loop period. 20ms
#define AA 0.97       // complementary filter constant
#define A_GAIN 0.0573 // [deg/LSB]
#define G_GAIN 0.070  // [deg/s/LSB]

// Constructor
IMU::IMU()
{
}

// Destructor
IMU::~IMU()
{
    // delete [] sensor_handle_,gyr_Xangle_,gyr_Yangle_, gyr_Zangle_,roll_,pitch_; // Delete private variables
    // delete [] rate_gyr_x_,rate_gyr_y_,rate_gyr_z_,mag_X_,mag_Y_,mag_Z_; // Delete private variables
}
void IMU::ConvertACCData(int aX, int aY, int aZ)
{

    printf("non-Converted - X: %d, Y: %d Z: %d\n", aX, aY, aZ);
  /*   if(aX > 0x8000) 
    {
        aX = aX - 0xFFFF;
    }
    else if (aY > 0x8000)
    {
        aY = aY - 0xFFFF;
    }

    else if (aZ > 0x8000)
    {
        aZ = aZ - 0xFFFF;
    }
     */

  /*   double XL_xdeg = (atan2(-aY, -aZ))/(PI/180);
    double XL_ydeg = (atan2(-aZ, -aX))/(PI/180);
    double XL_zdeg = 180 - (atan2(-aY, -aZ) / (PI / 180)); */

    XL_Roll_ = atan2(aY, aZ) * 180 / PI;
    XL_Pitch_ = atan2(-aX, sqrt(aY * aY + aZ * aZ)) * 180 / PI;

   /*  XL_xdeg -= (double)180.0;
    if (XL_ydeg > 90)
        XL_ydeg -= (double)270;
    else
        XL_ydeg += (double)90; 
     if (XL_xdeg >180)
        XL_xdeg -= (double)360.0;
*/
    printf("Converted - X: %lf, Y: %lf\n\n", XL_Roll_, XL_Pitch_);
}

void IMU::ConvertGyroData(int gX, int gY, int gZ)
{
    rate_gyr_x_ = (float)gX * G_GAIN;
    rate_gyr_y_ = (float)gY * G_GAIN;
    rate_gyr_z_ = (float)gZ * G_GAIN;
    printf("GyroX: %f, GyroY: %f, GyroZ: %f\n", rate_gyr_x_, rate_gyr_y_, rate_gyr_z_);
    gyroXangle_ += rate_gyr_x_ * DT;
    gyroYangle_ += rate_gyr_y_ * DT;
    gyroZangle_ += rate_gyr_z_ * DT;
    printf("GyroXangle: %f, GyroYangle: %f, GyroZangle: %f\n", gyroXangle_, gyroYangle_, gyroZangle_);
}

void IMU::ConvertMagData(int mY, int mX)
{
    magYaw_ = 180 * (atan2(mY,mX)/PI);
    if(magYaw_ < 0)
      magYaw_ += 360;
    printf("magYaw: %f\n\n", magYaw_);
}

/* float IMU::getACCData() // Returns ACC data
{
    return accData;
} */

float IMU::getGyroData() // Returns Gyro data
{
    return gyroYangle_;
}

float IMU::getMagData() // Returns Mag data
{
    return gyroXangle_;
}