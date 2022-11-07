#include "IMU.hpp"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Definitions
#define M_PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578
#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant
#define A_GAIN 0.0573    // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]

// Constructor
IMU::IMU()
{}

// Destructor
IMU::~IMU()
{
    //delete [] sensor_handle_,gyr_Xangle_,gyr_Yangle_, gyr_Zangle_,roll_,pitch_; // Delete private variables
    //delete [] rate_gyr_x_,rate_gyr_y_,rate_gyr_z_,mag_X_,mag_Y_,mag_Z_; // Delete private variables
}
void IMU::ConvertACCData(int aY, int aX, int aZ)
{

float XL_Sensitivity = 0.061; // +/-2g
double pi = 3.14159265358979;
double conv_Rate = 24/4369;
printf("non-Converted - X: %d, Y: %d Z: %d\n",aX,aY,aZ);
float aXg = (aX*XL_Sensitivity)/1000; // value in g
float aYg = (aY*XL_Sensitivity)/1000; // value in g
float aZg = (aZ*XL_Sensitivity)/1000; // value in g

double aXraw = aX*conv_Rate; 
double aYraw = aY*conv_Rate;
//double aZraw = aZ*conv_Rate;

double aXraw_off = (aX+0xFFFF)*conv_Rate; 
double aYraw_off = (aY+0xFFFF)*conv_Rate;
//double aZraw_off = aZ*conv_Rate+0xFFFF;

printf("Raw - X: %lf, Y: %lf \n",aXraw, aYraw);
printf("Raw_off - X: %f, Y: %f \n",aXraw_off, aYraw_off);
float A = sqrt(pow(aXg,2)+ pow(aYg,2) + pow(aZg,2)); // force on object
double XL_xdeg = (atan2(-aXg,-aZg))/(pi/180)+180;
double XL_ydeg = (atan2(-aYg,-aZg)/(pi/180))+180;
double XL_zdeg = 180-(atan2(-aYg,-aZg)/(pi/180));

printf("Converted - X: %f, Y: %f Z: %f\n",XL_xdeg,XL_ydeg, XL_zdeg);

}

void IMU::ConvertGyroData(int gY, int gX, int gZ)
{

}

void IMU::ConvertMagData(int mY, int mX, int mZ)
{
    
}

/* float IMU::getACCData() // Returns ACC data
{
    return accData;
} */

float IMU::getGyroData() // Returns Gyro data
{
    return gyroData;
}

float IMU::getMagData() // Returns Mag data
{
    return magData;
}