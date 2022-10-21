#include "I2C.hpp"
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"

int main()
{

// Open I2C connection
I1.I2C::openI2C(LSM6DSOX_ADDR2);

// Enable accelerometer
I1.I2C::WriteI2C(LSM6DSOX_CTRL1_XL, 0b10100000);
I1.I2C::WriteI2C(LSM6DSOX_CTRL3_C, 0b01000000);

// Enable gyro
I1.I2C::WriteI2C(LSM6DSOX_CTRL2_G, 0b10101100);

// Main loop
while(1)
{
    int ACC_x = I1.I2C::ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A)};
    int ACC_y = I1.I2C::ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A)};
    int ACC_z = I1.I2C::ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A)};

    IMU2.IMU::ConvertIMUData(ACC_x, ACC_y, ACC_z, 1); //Accelerometer conversion
    printf("Roll: %f \nPitch: %f \n", IMU2.roll_, IMU2.pitch_) //Print accelerometer data

    usleep(1000000); //Small delay
}

return 0;
}