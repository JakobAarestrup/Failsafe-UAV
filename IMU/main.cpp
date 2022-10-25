#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "I2C.hpp"
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <wiringPiI2C.h>
#include <wiringPi.h>

int main()
{

I2C I1;
IMU IMU2;

// Open I2C connection
//I1.openI2C(LSM6DSOX_ADDR2);

// Enable accelerometer
//I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL1_XL, 0b10100000);
//I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000);

// Enable gyro
//I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL2_G, 0b10101100);

// Enable magnetometer - 
//I1.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG1, 0b01011100);
//I1.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG2, 0b01100000);



// Main loop
while(1)
{
    // Open I2C connection

I1.openI2C(LSM6DSOX_ADDR2);

// Enable accelerometer
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL1_XL, 0b10100000);
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000);

// Enable gyro
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL2_G, 0b10101100);

    int ACC_x = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A);
    int ACC_y = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A);
    int ACC_z = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A);
    int GYR_x = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G);
    int GYR_y = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G);
    int GYR_z = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G);

    IMU2.ConvertIMUData(ACC_x, ACC_y, ACC_z, 1); //Accelerometer conversion
    IMU2.ConvertIMUData(GYR_x, GYR_y, GYR_z, 2); //Accelerometer conversion

    float roll = IMU2.getRoll();
    float pitch = IMU2.getPitch();
    float GYR_X = IMU2.getGyrX();
    float GYR_Y = IMU2.getGyrY();
    float GYR_Z = IMU2.getGyrZ();

    printf("Roll: %f    Pitch: %f \n", roll, pitch); //Print accelerometer data
    printf("GYR_X: %f   GYR_Y: %f   GYR_Z: %f \n", GYR_X, GYR_Y, GYR_Z); //Print gyroscope data

    I1.openI2C(LIS3MDL_ADDR_2);

    // Enable magnetometer - 
    I1.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
    I1.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
    I1.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
    I1.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)
    
    int MAG_x = I1.ReadI2C(LIS3MDL_ADDR_2, LIS3MDL_OUT_X_L);
    int MAG_y = I1.ReadI2C(LIS3MDL_ADDR_2, LIS3MDL_OUT_Y_L);
    int MAG_z = I1.ReadI2C(LIS3MDL_ADDR_2, LIS3MDL_OUT_Z_L);
    //printf("MAG_x: %d \nMAG_y: %d \nMAG_z: %d \n", MAG_x, MAG_y, MAG_z); //Print mag data

    IMU2.ConvertIMUData(MAG_x, MAG_y, MAG_z, 3); //Magnometer conversion
    int MAG_X = IMU2.getMagX();
    int MAG_Y = IMU2.getMagY();
    int MAG_Z = IMU2.getMagZ();

    printf("MAG_X: %d       MAG_Y: %d       MAG_Z: %d \n", MAG_X, MAG_Y, MAG_Z); //Print mag data
    
    usleep(1000000); //Small delay
}

return 0;
}