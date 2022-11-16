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
int gyro_sensitivity = 
IMU IMU1;

// Enable magnetometer from IMU1

// Main loop
while(1)
{
     // Read from Gyro from IMU2
    float gx = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G);
    float gy = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G);
    float gz = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G);

    

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

}
return 0;
}