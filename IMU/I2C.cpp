#include "I2C.hpp"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
extern "C"
{
    #include <linux/i2c-dev.h>
}

/*Configurations for registers*/
#define ACC_Config = 0b10100000;
#define GYR_Config = 0b10101100;
#define IMU_Config = 0b01000000;

// Constructor
I2C::I2C()
{}

// Destructor
I2C::~I2C()
{
    //delete [] I2C_Data_,I2C_File_,Write_File_; // Delete private variables
}

// Open i2c communication
void I2C::openI2C(int ADDR)
{
    I2C_File_ = wiringPiI2CSetup(ADDR); // Open I2C communication
    if (I2C_File_ ==-1) // Check error
    {
        printf("Failed to establish I2C connection."); // Failed to open I2C communication
        exit(1);
    }
}

// Write to I2C device register
void I2C::WriteI2C(int ADDR, int reg, int data)
{
    Write_File_ = wiringPiI2CWriteReg8(I2C_File_, reg, data); // Write to I2C device register
    if (Write_File_ ==-1) // check error
    {
        printf("Failed to write to %d via I2C.", ADDR); // Write error
        exit(1);
    }
}

// Read from i2c device register
int I2C::ReadI2C_16bit(int ADDR, int reg)
{
    int Reg_data1 = wiringPiI2CReadReg8(I2C_File_, reg); // Read first register
    int Reg_data2 = wiringPiI2CReadReg8(I2C_File_, reg+1); // Read second register

    if (Reg_data1 ==-1 | Reg_data2 ==-1) // Check read errors
    {
        printf("Failed to read from %d via I2C.", ADDR); // Read error
        exit(1);
    }
    else
    {  
        int I2C_Data_ = (Reg_data1 | Reg_data2 << 8); // Bitshift registers together
        if(I2C_Data_ < 0x8000) 
        {

        }
        else
        {
            I2C_Data_ = I2C_Data_ - 0xFFFF;
        }

        return I2C_Data_; // Return i2c data
    }  
}

// Read from i2c device register
int I2C::ReadI2C_8bit(int ADDR, int reg)
{
    int Reg_data_ = wiringPiI2CReadReg8(I2C_File_, reg); // Read first register

    if (Reg_data_ ==-1) // Check read errors
    {
        printf("Failed to read from %d via I2C.", ADDR); // Read error
        exit(1);
    }
    else
   {
        return Reg_data_; // Return i2c data
   }
}