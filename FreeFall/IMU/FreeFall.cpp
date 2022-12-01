#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <wiringPi.h>
#include <stdbool.h>
#include "I2C.hpp"
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"

#define DT 0.02 // [s/loop] loop period. 20ms


int main()
{
  I2C I2;
  IMU IMU2;

  I1.initializeI2C();
  int freeFall = 0;


  // Main loop
  while (1)
  {
    free fall = IMU2.freeFall(2);
    printf("Free fall detection: \i\n",freeFall);
  }

  return 0;
}