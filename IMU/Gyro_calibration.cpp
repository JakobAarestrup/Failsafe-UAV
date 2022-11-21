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
int gyro_sensitivity = 8.75;
int DT = 20;
int count = 0;

// Enable  from IMU2
//I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL2_G, 0b01010000);   // Gyro = 208 Hz (normal mode) 250 dps
I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000);   // Enable BDU
//I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL4_C, 0b00000010);  // Gyro LPF enabled 33        
I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL7_G, 0b01000000);  // enable HPF // default HPF (00) 16 Hz

int startofloop  = mymillis();
struct  timeval tvBegin, tvEnd,tvDiff;
gettimeofday(&tvBegin, NULL);

// Main loop
   // while(mymillis() < startofloop + 200)
        // Read from Gyro from IMU2
        while(1)
        {
        startofloop = mymillis();
        
        float gx = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G,1,1);
        float gy = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G,1,1);
        float gz = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G,1,1);

        float gxc = ((gx/gyro_sensitivity) / 57.3)/1000; // rad/s
        float gyc = ((gy/gyro_sensitivity) / 57.3)/1000; // rad/s
        float gzc = ((gz/gyro_sensitivity) / 57.3)/1000; // rad/s
        

        printf("%0.5f %0.5f %0.5f \n",gxc,gyc,gzc);
         while(mymillis() - startofloop < DT)
         {
            usleep(100);
         }
         count++;
         if (count == 2000)
         {
            break;
         }
        }

return 0;

}