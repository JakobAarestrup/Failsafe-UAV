#include <stdio.h>
#include <unistd.h>
#include <iostream>
//#include "I2C.hpp"
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include "I2C_v2.hpp"
//#include "Kalman.cpp"

#define DT 0.02         // [s/loop] loop period. 20ms (2s)

int mymillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int main()
{

I2C I1;
I2C I2;
IMU IMU1;
//Kalman k1;
int startofloop  = mymillis();
struct  timeval tvBegin, tvEnd,tvDiff;
gettimeofday(&tvBegin, NULL);

I1.initializeI2C();

// Main loop
while(1)
{
    startofloop = mymillis();
    float ax = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A,2,1);
    float ay = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A,2,1);
    float az = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A,2,1);
    float gx = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G,2,1);
    float gy = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G,2,1);
    float gz = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G,2,1);
   /*  float mx = I2.ReadI2C_16bit(LIS3MDL_ADDR2, LIS3MDL_OUT_X_L);
    float my = I2.ReadI2C_16bit(LIS3MDL_ADDR2, LIS3MDL_OUT_Y_L);
    float mz = I2.ReadI2C_16bit(LIS3MDL_ADDR2, LIS3MDL_OUT_Z_L); */
    
    IMU1.ConvertACCData(ax, ay, az);
    IMU1.ConvertGyroData(gx,gy,gz);
    //IMU1.ConvertMagData(mx,my);

  while(mymillis() - startofloop < 20){
            usleep(100);
    }
    printf("Loop Time %d\n", mymillis()- startofloop);
}
return 0;
}