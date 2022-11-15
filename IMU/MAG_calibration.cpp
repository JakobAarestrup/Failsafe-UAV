#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include "I2C.hpp"
#include "I2Cdev.h"

int main()
{

I2C I1;

int mag_gain = 6842;

// Enable magnetometer
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG1, 0b00100010);// OM = 01 (Medium-performance mode for X and Y); DO = 000 (default) FAST_ODR = 1 (155 Hz ODR)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG4, 0b00000100);// OMZ = 11 (Medium-performance mode for Z)

// Main loop
while(1)
{
    float mx = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_X_L,1,1);
    float my = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Y_L,1,1);
    float mz = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Z_L,1,1);

    if(mx > 0x8000) 
    {
        mx = mx - 0xFFFF;
    }

    if (my > 0x8000)
    {
        my = my - 0xFFFF;
    }
    
    if (mz > 0x8000)
    {
        mz = mz - 0xFFFF; // lave det her i I2C read kald?
    } 

    float mxc = (mx/mag_gain)*100;
    float myc = (my/mag_gain)*100;
    float mzc = (mz/mag_gain)*100;
    
    printf("%f %f %f \n",mxc,myc,mzc);
    usleep(10000); // 10ms Delay
}
return 0;
}