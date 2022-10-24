#include "IMU.hpp"
#include <math.h>

#define M_PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578
#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant
#define A_GAIN 0.0573    // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]

IMU::IMU()
{}

IMU::~IMU()
{}

void IMU::ConvertIMUData(int x, int y, int z, int sensor_handle)
{    
    if (sensor_handle == 1) // Accelerometer
    {
        double x_ACC = float(x);
        double y_ACC = float(y);
        double z_ACC = float(z);
        this->roll_ = atan2(y_ACC , z_ACC) * 57.3;
        this->pitch_ = atan2((- x_ACC), sqrt(y_ACC * y_ACC + z_ACC * z_ACC)) * 57.3;
    }
    else if (sensor_handle == 2) // Gyroscope
    {
        this->rate_gyr_x_ = float(x) * G_GAIN;
	    this->rate_gyr_y_ = float(y) * G_GAIN;
	    this->rate_gyr_z_ = float(z) * G_GAIN;

        //Calculate the angles from the gyro
	    this->gyr_Xangle_ = this->rate_gyr_x_*DT;
	    this->gyr_Yangle_ = this->rate_gyr_y_*DT;
	    this->gyr_Zangle_ = this->rate_gyr_z_*DT;
    }
    else if (sensor_handle == 3) // Magnetometer
    {
        mag_X_ = x;
        mag_Y_ = y;
        mag_Z_ = z;
    }
}

double IMU::getRoll()
{
    return roll_;
}

double IMU::getPitch()
{
    return pitch_;
}

int IMU::getGyrX()
{
    return gyr_Xangle_;
}

int IMU::getGyrY()
{
    return gyr_Yangle_;
}

int IMU::getGyrZ()
{
    return gyr_Zangle_;
}

int IMU::getMagX()
{
    return mag_X_;
}

int IMU::getMagY()
{
    return mag_Y_;
}

int IMU::getMagZ()
{
    return mag_Z_;
}