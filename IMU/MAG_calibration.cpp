#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include "I2C.hpp"

int main()
{

I2C I1;

// Enable magnetometer
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)

// Main loop
while(1)
{
    float mx = I1.ReadI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_X_L,1,1);
    float my = I1.ReadI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Y_L,1,1);
    float mz = I1.ReadI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Z_L,1,1);
    
    printf("%f, %f, %f\n",mx,my,mz);
}
return 0;
}