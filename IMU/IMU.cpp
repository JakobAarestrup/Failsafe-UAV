#include "I2C.h"
#include "LIS3MDL.h"
#include "LSM6DSOX.h"

#define M_PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578
#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant
#define A_GAIN 0.0573    // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]


double ConvertIMUData(int x, int y, int z, int sensor_handle)
{    
    if (sensor_handle == 1) // Accelerometer
    {
        double x_ACC = float(x);
        double y_ACC = float(y);
        double z_ACC = float(z);
        roll_ = atan2(y_ACC , z_ACC) * 57.3;
        pitch_ = atan2((- x_ACC), sqrt(y_ACC * y_ACC + z_ACC * z_ACC)) * 57.3;
    }
    else if (sensor_handle == 2) // Gyroscope
    {
        rate_gyr_x_ = float(x) * G_GAIN;
	    rate_gyr_y_ = float(y) * G_GAIN;
	    rate_gyr_z_ = float(z) * G_GAIN;

        //Calculate the angles from the gyro
	    gyr_Xangle_ = rate_gyr_x_*DT;
	    gyr_Yangle_ = rate_gyr_y_*DT;
	    gyr_Zangle_ = rate_gyr_z_*DT;
    }
    else if (sensor_handle == 3) // Magnetometer
    {

    }
}