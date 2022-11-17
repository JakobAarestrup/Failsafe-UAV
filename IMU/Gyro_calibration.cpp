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
int gyro_sensitivity = 17.5;

// Enable  from IMU2
I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL2_G, 0b01100100);   // Gyro = 416 Hz (High-Performance mode) 500 dps
I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000);   // Enable BDU

// Main loop
    while(1)
    {
        // Read from Gyro from IMU2
        float gx = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G,1,1);
        float gy = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G,1,1);
        float gz = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G,1,1);

        float gxc = (gx*gyro_sensitivity)/1000; 
        float gyc = (gy*gyro_sensitivity)/1000; 
        float gzc = (gz*gyro_sensitivity)/1000; 

        if (N < 500)
        {
            printf("%f %f %f \n",gx,gy,gz);
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