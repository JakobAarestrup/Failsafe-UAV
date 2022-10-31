#include "IMU.hpp"
#include <math.h>
#include "Fusion-main/Fusion.h"
#include <stdbool.h>
#include <stdio.h>

/*Math constants*/
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
    delete [] sensor_handle_,gyr_Xangle_,gyr_Yangle_, gyr_Zangle_,roll_,pitch_; // Delete private variables
    delete [] rate_gyr_x_,rate_gyr_y_,rate_gyr_z_,mag_X_,mag_Y_,mag_Z_; // Delete private variables
}

// Convert data from IMU to usable data
void IMU::ConvertIMUData(int x, int y, int z, int sensor_handle)
{    
    if (sensor_handle == 1) // Accelerometer sensor handle
    {
        float x_ACC = float(x); // Convert x to float
        float y_ACC = float(y); // Convert y to float
        float z_ACC = float(z); // Convert z to float
        roll_ = atan2(y_ACC , z_ACC) * RAD_TO_DEG; // Calculate roll angle
        pitch_ = atan2((- x_ACC), sqrt(y_ACC * y_ACC + z_ACC * z_ACC)) * RAD_TO_DEG; // Calculate pitch angle
    }
    else if (sensor_handle == 2) // Gyroscope sensor handle
    {
        rate_gyr_x_ = float(x) * G_GAIN; // Convert x to float and multiply by gain
	    rate_gyr_y_ = float(y) * G_GAIN; // Convert y to float and multiply by gain
	    rate_gyr_z_ = float(z) * G_GAIN; // Convert z to float and multiply by gain

        //Calculate the angles from the gyro
	    gyr_Xangle_ = rate_gyr_x_*DT;
	    gyr_Yangle_ = rate_gyr_y_*DT;
	    gyr_Zangle_ = rate_gyr_z_*DT;
    }
    else if (sensor_handle == 3) // Magnetometer sensor handle
    {
        mag_X_ = x;
        mag_Y_ = y;
        mag_Z_ = z;
    }
}

// Returns roll
float IMU::getRoll() const
{
    return roll_;
}
 // Returns pitch
float IMU::getPitch() const
{
    return pitch_;
}
// Returns yaw
float IMU::getYaw() const
{
    return yaw_;
}