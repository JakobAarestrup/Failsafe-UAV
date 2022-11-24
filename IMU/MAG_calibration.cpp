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
int N = 0;
int mag_gain = 6842;
IMU IMU1;

// Enable magnetometer from IMU1
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG1, 0b01100010);// OM = 11 (ultra-high-performance mode for X and Y) FAST_ODR = 1 (155Hz ODR)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale) Default
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)
I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)
/* Enable Magnetometer from IMU2
I2Cdev::writeByte(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG1, 0b01100010);// OM = 11 (ultra-high-performance mode for X and Y) FAST_ODR = 1 (155Hz ODR)
I2Cdev::writeByte(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale) Default
I2Cdev::writeByte(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2Cdev::writeByte(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)*/

// Main loop
while(1)
{
    
    // Read from Magnetometer from IMU1
    //float mx = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_X_L,1,1);
    //float my = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Y_L,1,1);
    //float mz = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Z_L,1,1);

    /* Read from the Magnetometer from IMU2
    float mx = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_X_L,1,1);
    float my = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_Y_L,1,1);
    float mz = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_Z_L,1,1);
    */

    /*
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

    if (N < 2000)
    {
        printf("%f %f %f \n",mxc,myc,mzc);
        usleep(20000); // 20ms Delay
        N = N+1;
    }
    else
    {
        printf("done!!!");
        break;
    }
    */

   float mx = 1;
   float my = 1;

   IMU1.readIMU();
   IMU1.ConvertMagData(mx,my);
   usleep(100000);
}
return 0;
}