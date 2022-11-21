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
int mymillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int main()
{



I2C I1;
int N = 0;
int gyro_sensitivity = 70;
int DT = 20;

// Enable  from IMU2
I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL2_G, 0b01011100);   // Gyro = 208 Hz (normal mode) 2000 dps
I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL3_C, 0b01000000);   // Enable BDU
I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL7_G, 0b01000000);  // enable HPF // default HPF (00) 16 Hz

int startofloop  = mymillis();
struct  timeval tvBegin, tvEnd,tvDiff;
gettimeofday(&tvBegin, NULL);

// Main loop
   // while(mymillis() < startofloop + 200)
        // Read from Gyro from IMU2
        while(1)
        {
        startofloop = mymillis();
        
        float gx = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_G,1,1);
        float gy = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_G,1,1);
        float gz = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_G,1,1);

        float gxc = (gx*gyro_sensitivity)/1000; 
        float gyc = (gy*gyro_sensitivity)/1000; 
        float gzc = (gz*gyro_sensitivity)/1000; 
        

        printf("%f %f %f \n",gx,gy,gz);
         while(mymillis() - startofloop < DT){
            usleep(100);
         }
        }

return 0;

}