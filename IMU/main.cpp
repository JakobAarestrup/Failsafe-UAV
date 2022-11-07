#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "I2C.hpp"
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <time.h>
#include "Kalman2.cpp"

#define dt (200) // replace this with actual sample rate

int main()
{
I2C I1;
I2C I2;
IMU IMU1;
Kalman2 k1;
int dt_new = 0;

// Open I2C connection
I1.openI2C(LSM6DSOX_ADDR2);
I2.openI2C(LIS3MDL_ADDR_2);

// Enable accelerometer and gyroscope
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL2_G, 0b10011100); // Gyro = 416 Hz (High-Performance mode)
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL1_XL, 0b011000000); // Acc = 416 Hz (High-Performance mode)
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000); // Enable BDU
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL7_G, 0b10000000); // Enable High-Performance mode for Gyro

// Enable magnetometer
I2.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
I2.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
I2.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)

//Test
float acc_roll = 90;
float acc_pitch = 90;
float gyro_roll = 90;
float gyro_pitch = 90; 
float gyro_yaw = 90; 
float mag_yaw = 90;

// Main loop
while(1)
{
    if (dt - dt_new >= 200)
    {
        dt_new = dt; //200ms in first run. 400 in second run etc.

        /*
        float ax = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A);
        float ay = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A);
        //float az = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A);
        float gx = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G);
        float gy = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G);
        float gz = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G);
        //float mx = I2.ReadI2C_16bit(LIS3MDL_ADDR_2, LIS3MDL_OUT_X_L);
        //float my = I2.ReadI2C_16bit(LIS3MDL_ADDR_2, LIS3MDL_OUT_Y_L);
        float mz = I2.ReadI2C_16bit(LIS3MDL_ADDR_2, LIS3MDL_OUT_Z_L);

        //IMU1.ConvertACCData(ax, ay, az);
        //IMU1.ConvertGYROData(gx, gy, gz);
        //IMU1.ConvertMagData(mx, my, mx);

        float acc_roll = IMU1.ConvertACCData(ax);
        float acc_pitch = IMU1.ConvertACCData(ay);
        float gyro_roll = IMU1.ConvertGYROData(gx);
        float gyro_pitch = IMU1.ConvertGYROData(gy);
        float gyro_yaw = IMU1.ConvertGYROData(gz);
        float mag_yaw = IMU1.ConvertMAGData(mz);
        */

        

        float KFRoll = k1.getAngle(acc_roll, gyro_roll, dt);
        float KFPitch = k1.getAngle(acc_pitch, gyro_pitch, dt);
        float KFYaw = k1.getAngle(mag_yaw, gyro_yaw, dt);
    
        printf("Roll: %f Pitch: %f Yaw: %f\n", KFRoll, KFPitch, KFYaw);
        dt = 0; // Reset DT
        acc_roll = acc_roll + 10;
        acc_pitch = acc_pitch + 15;
        gyro_roll = gyro_roll + 5;
        gyro_pitch = gyro_pitch + 5; 
        gyro_yaw = gyro_yaw + 5; 
        mag_yaw = mag_yaw + 20;
    }
    else 
    {
        dt = millis();
    }
}

return 0;
}