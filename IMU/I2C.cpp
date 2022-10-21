#include "I2C.hpp"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
extern "C"
{
    #include <linux/i2c-dev.h>
}

#define ACC_Config = 0b10100000;
#define GYR_Config = 0b10101100;
#define IMU_Config = 0b01000000;

I2C()
{}

~I2C()
{}

void openI2C(int ADDR)
{
    int I2C_File_ = wiringPiI2CSetup(ADDR);
    if (I2C_File_ ==-1)
    {
        printf("Failed to establish I2C connection.");
        exit(1);
    }
    printf("Succesfully setup I2C connection.\n");
}

void WriteI2C(int reg, int data)
{
    int Write_File_ = wiringPiI2CWriteReg8(I2C_File_, reg, data);
    if (Write_File_ ==-1)
    {
        printf("Failed to write to %d via I2C.", ADDR);
        exit(1);
    }
    printf("Succesfully written to register via I2C.\n");
}

int ReadI2C(int ADDR, int reg)
{
    int Reg_data1 = wiringPiI2CReadReg8(I2C_File_, reg);
    int Reg_data2 = wiringPiI2CReadReg8(I2C_File_, reg+1);

    if (Reg_data1 ==-1 | Reg_data2 ==-1)
    {
        printf("Failed to read from %d via I2C.", ADDR);
        exit(1);
    }
    else
   {
        int I2CData_ = (Reg_data2 << 8) | Reg_data1;
        return 0;
   }
}