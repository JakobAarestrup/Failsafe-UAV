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
#define G_GAIN 0.07  // [deg/s/LSB]

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
void IMU::ConvertACCData(float aX, float aY, float aZ)
{

    /* double aXf = float(aX);
    double aYf = float(aY);
    double aZf = float(aZ); 
 */
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

   
/*  double XL_xdeg = (atan2(-aY, -aZ))/(PI/180);
    double XL_ydeg = (atan2(-aZ, -aX))/(PI/180);
    double XL_zdeg = 180 - (atan2(-aY, -aZ) / (PI / 180)); */

    float aXf = (aX*0.061)/1000;
    float aYf = (aY*0.061)/1000;
    float aZf = (aZ*0.061)/1000;

    XL_Roll_ = atan2(aYf, aZf) * 180 / PI;
    XL_Pitch_ = atan2(-aXf, sqrt(aYf * aYf + aZf * aZf)) * 180 / PI;
    printf("Converted - XL_Roll: %lf, XL_pitch: %lf\n\n", XL_Roll_, XL_Pitch_);


/*     double conv_rate = 90.053243980503472678;

    double aXc = conv_rate *aXf;
    double aYc = conv_rate *aYf;
    double aZc = conv_rate *aZf;

    //printf("Bias - roll: %lf, pitch: %lf\n\n", XL_Roll_, XL_Pitch_);
    printf("Conv - X-AXIS: %lf, Y-AXIS: %lf Z-AXIS: %lf\n\n", aXc, aYc, aZc);   */
}

void IMU::ConvertGyroData(float gX, float gY, float gZ)
{
    rate_gyr_x_ = (gX * G_GAIN)/1000;
    rate_gyr_y_ = (gY * G_GAIN)/1000;
    rate_gyr_z_ = (gZ * G_GAIN)/1000;
    //printf("GyroX: %f, GyroY: %f, GyroZ: %f\n", rate_gyr_x_, rate_gyr_y_, rate_gyr_z_);
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