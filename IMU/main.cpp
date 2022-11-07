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
#include "Kalman2.hpp"

//#define dt (2000) // replace this with actual sample rate

int main()
{
I2C I1;
I2C I2;
IMU IMU1;
Kalman2 k1;
Kalman2 k2;
Kalman2 k3;
float T_old = 0;
float dT = 2000;

// Open I2C connection
I1.openI2C(LSM6DSOX_ADDR1);
I2.openI2C(LIS3MDL_ADDR_1);
float T = millis();

// Enable accelerometer and gyroscope
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL2_G, 0b10011100); // Gyro = 416 Hz (High-Performance mode)
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL1_XL, 0b011000000); // Acc = 416 Hz (High-Performance mode)
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL3_C, 0b01000000); // Enable BDU
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL7_G, 0b10000000); // Enable High-Performance mode for Gyro

// Enable magnetometer
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)

//Test
k1.setAngle(85); // roll
k2.setAngle(80); // pitch
k3.setAngle(75); // yaw
float acc_roll =80;
float acc_pitch =80;
float gyro_roll = 90;
float gyro_pitch = 90; 
float gyro_yaw = 90; 
float mag_yaw = 75;

// Main loop
while(1)
{
    if (T - T_old >= dT)
    {
        T_old = T; //2000ms in first run. 4000 in second run etc.

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
        float KFPitch = k2.getAngle(acc_pitch, gyro_pitch, dt);
        float KFYaw = k3.getAngle(mag_yaw, gyro_yaw, dt);
    
        printf("Roll: %f Pitch: %f Yaw: %f\n", KFRoll, KFPitch, KFYaw);
        

        if (gyro_roll == 360)
        {
            acc_roll = 0;
            gyro_roll = 0;
        }
        else
        {
            acc_roll = acc_roll + 5;
            gyro_roll = gyro_roll + 10;
        }

        if (gyro_pitch == 360)
        {
            acc_pitch = 0;
            gyro_pitch = 0;
        }
        else
        {
            acc_pitch = acc_pitch + 5;
            gyro_pitch = gyro_pitch + 15;
        }
            
        if (gyro_yaw == 360)
        {
            mag_yaw = 0;
            gyro_yaw = 0;
        }
        else
        {
            mag_yaw = mag_yaw + 5;
            gyro_yaw = gyro_yaw + 20; 
        }
        printf("Time passed in ms: %f\n", dt);
        T = 0; // Reset dt
    }
    else 
    {
        T = millis();
    }
}

return 0;
}