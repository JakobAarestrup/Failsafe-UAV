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
//#define devID  1;


void openI2C(int devID)
{
    int result = wiringPiI2CSetup(devID);
    if (result ==-1)
    {
        printf("Failed to establish I2C connection.");
        exit(1);
    }
    printf("Succesfully setup I2C connection.\n");
}

void WriteI2C(int ADDR, int reg, int data)
{
    int wd = wiringPiI2CWriteReg8(ADDR, reg, data);
    if (wd ==-1)
    {
        printf("Failed to write to %d via I2C.", ADDR);
        exit(1);
    }
    printf("Succesfully written to register via I2C.\n");
}

void ReadI2C(int fd, int ADDR, int reg)
{
    int rd1 = wiringPiI2CReadReg8(fd, reg);
    int rd2 = wiringPiI2CReadReg8(fd, reg+1);

    if (rd1 ==-1 | rd2 ==-1)
    {
        printf("Failed to read from %d via I2C.", ADDR);
        exit(1);
        
    }
    else
   {
        int I2CData = (rd2 << 8) | rd1;
        //printf("rd1 is: %d and rd2 is: %d \n",rd1,rd2);
        printf("Data from %d is: %d \n",ADDR, I2CData);
        return I2CData;
   }
}



int main()
{
//int file_i2c;
//int* acc_raw;
//float AccXangle;
//float AccYangle;
const int ADDR = LSM6DSOX_ADDR2;
const int reg1 = LSM6DSOX_CTRL1_XL;
const int reg2 = LSM6DSOX_CTRL3_C;
const int reg3 = LSM6DSOX_OUT_X_L_A;
const int reg4 = LSM6DSOX_OUT_Y_L_A;

printf("Adresse: %d", ADDR);

int fd = wiringPiI2CSetup(ADDR);
    if (fd ==-1)
    {
        printf("Failed to establish I2C connection.");
        exit(1);
    }
    printf("Succesfully setup I2C connection.\n");

// Open I2C Connection via DevADDR
//int fd = openI2C(ADDR);

// Enable accelerometer
WriteI2C(fd, reg1, 0b10100000);
WriteI2C(fd, reg2, 0b01000000);

while(1)
{
    int ACC_X {ReadI2C(fd, ADDR, LSM6DSOX_OUT_X_L_A)};
    int ACC_Y {ReadI2C(fd, ADDR, LSM6DSOX_OUT_Y_L_A)};

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