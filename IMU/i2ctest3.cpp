#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include "LIS3MDL.h"
#include "LSM6DOX.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
// #include <linux/i2c-dev.h>
//#include <i2c/smbus.h>
extern "C" {
    #include <linux/i2c-dev.h>
    #include <i2c/smbus.h>
}

#define M_PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578


void openI2C()
{
    int result = wiringPiI2CSetup(devID);
    if (result >0)
    {
        printf("Failed to read block from I2C.");
        exit(1);
    }
}

void WriteI2C(int ADDR, int reg, int data)
{
    int fd = wiringPiI2CWriteReg8(int ADDR, int reg, data);
    if (fd >0)
    {
        printf("Failed to write to %f via I2C.", ADDR);
        exit(1);
    }
}

void ReadI2C(int ADDR, int reg)
{
    int fd = wiringPiI2CReadReg8(int ADDR, int reg);
    if (fd >0)
    {
        printf("Failed to read from %f via I2C.", ADDR);
        exit(1);
    }
    else
    {
        int I2CData = fd;
        std::cout<<"Data from: "<<ADDR<<I2CData<<"\n";
    }
}


int main()
{
//int file_i2c;
//int* acc_raw;
//float AccXangle;
//float AccYangle;
const int devID = 1;
const int ADDR = LSM6DSOX_ADDR2;
const int reg1 = LSM6DSOX_CTRL1_XL;
const int reg2 = LSM6DSOX_CTRL3_C;
const int reg3 = LSM6DSOX_OUT_X_L_A;
const int reg4 = LSM6DSOX_OUT_X_H_A;

// Open I2C Connection via DevID
openI2C();

// Enable accelerometer
WriteI2C(ADDR, reg1, 0b10100000);
WriteI2C(ADDR, reg2, 0b01000000);

while(1)
{
    ReadI2C(ADDR, reg3);
    ReadI2C(ADDR, reg4);

    //Convert Accelerometer values to degrees
    //AccXangle = (float) (atan2(*(acc_raw+1),*(acc_raw+2))+M_PI)*RAD_TO_DEG;
    //AccYangle = (float) (atan2(*(acc_raw+2),*acc_raw)+M_PI)*RAD_TO_DEG;


    //Change the rotation value of the accelerometer to -/+ 180
   // if (AccXangle > 180)
   //     {      
   //         AccXangle -= (float)360.0;
   //     }

   // if (AccYangle > 180)
   //     {
   //         AccYangle -= (float)360.0;
   //     }
  //  printf("X-angle: %f", AccXangle);
  //  printf("Y-angle: %f", AccYangle);
    usleep(1000000);
}

return 0;
}