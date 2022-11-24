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

#define DT 0.02 // [s/loop] loop period. 20ms

int mymillis()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

int main()
{

  I2C I1;
  I2C I2;
  IMU IMU1;
  IMU IMU2;

  int startofloop = mymillis();
  struct timeval tvBegin, tvEnd, tvDiff;
  gettimeofday(&tvBegin, NULL);

  I1.initializeI2C();
  // I2.initializeI2C();

  /*Not used atm*/
  IMU1.calibrateGyro();
  // IMU2.calibrateGyro();

  // Main loop
  while (1)
  {
    startofloop = mymillis();

    IMU1.readIMU();

    IMU1.ConvertACCData();
    IMU1.ConvertMagData();

    IMU1.ComplementaryFilter();

    /* IMU1.readIMU();

    IMU2.ConvertACCData();
    IMU2.ConvertMagData();

    IMU2.ComplementaryFilter();
 */

    while (mymillis() - startofloop < 20)
    {
      usleep(100);
    }

    printf("Loop Time %d\n", mymillis() - startofloop);
  }
  return 0;
}