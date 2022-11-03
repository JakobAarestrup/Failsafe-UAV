#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "I2C.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <time.h>

//#define SAMPLE_RATE (100) // replace this with actual sample rate

int main()
{

I2C I1;
I2C I2;
//FusionAhrs ahrs;

// Open I2C connection
I1.openI2C(LSM6DSOX_ADDR1);
I2.openI2C(LIS3MDL_ADDR_1);

// Enable accelerometer and gyroscope
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL2_G, 0b10011100); // Gyro = 416 Hz (High-Performance mode)
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL1_XL, 0b011000000); // Acc = 416 Hz (High-Performance mode)
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000); // Enable BDU
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL7_G, 0b10000000); // Enable High-Performance mode for Gyro

// Enable magnetometer
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)

// Main loop
while(1)
{
    float ax = I1.ReadI2C_16bit(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_A);
    float ay = I1.ReadI2C_16bit(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_A);
    float az = I1.ReadI2C_16bit(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_A);
    float gx = I1.ReadI2C_16bit(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_G);
    float gy = I1.ReadI2C_16bit(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_G);
    float gz = I1.ReadI2C_16bit(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_G);
    printf("AccX: %f AccY: %f AccZ: %f\n", ax, ay, az);
    printf("GyroX: %f GyroY: %f GyroZ: %f\n", gx,gy,gz);

    float mx = I2.ReadI2C_16bit(LIS3MDL_ADDR_1, LIS3MDL_OUT_X_L);
    float my = I2.ReadI2C_16bit(LIS3MDL_ADDR_1, LIS3MDL_OUT_Y_L);
    float mz = I2.ReadI2C_16bit(LIS3MDL_ADDR_1, LIS3MDL_OUT_Z_L);

    
    
    //printf("MagX: %f MagY: %f MagZ: %f\n", mx, my, mz);
    usleep(1000000);
}
return 0;
}