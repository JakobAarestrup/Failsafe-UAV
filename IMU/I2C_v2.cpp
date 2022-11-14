#include "I2C_v2.hpp"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include "I2Cdev.h"
extern "C"
{
    #include <linux/i2c-dev.h>
}

// Constructor
I2C::I2C()
{}

// Destructor
I2C::~I2C()
{
    //delete [] I2C_Data_,I2C_File_,Write_File_; // Delete private variables
}

// Open i2c communication
void I2C::initializeI2C()
{
    //First IMU:
    I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
    I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL2_G, 0b10011100); // Gyro = 416 Hz (High-Performance mode)
    I2Cdev::writeByte(LIS3MDL_ADDR1, LSM6DSOX_CTRL1_XL, 0b011000000); // Acc = 416 Hz (High-Performance mode)
    I2Cdev::writeByte(LIS3MDL_ADDR1, LSM6DSOX_CTRL3_C, 0b01000000); // Enable BDU
    I2Cdev::writeByte(LIS3MDL_ADDR1, LSM6DSOX_CTRL7_G, 0b10000000); // Enable High-Performance mode for Gyro

    I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
    I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
    I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
    I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)

    //Second IMU:
   /*  I2Cdev::writeBytes(LSM6DSOX_ADDR2, LSM6DSOX_INT1_CTRL, 1, 0b00000011); // Enable gyroscope and accelerometer data interrupt
    I2Cdev::writeBytes(LSM6DSOX_ADDR2, LSM6DSOX_CTRL2_G, 1, 0b10011100); // Gyro = 416 Hz (High-Performance mode)
    I2Cdev::writeBytes(LIS3MDL_ADDR2, LSM6DSOX_CTRL1_XL, 1, 0b011000000); // Acc = 416 Hz (High-Performance mode)
    I2Cdev::writeBytes(LIS3MDL_ADDR2, LSM6DSOX_CTRL3_C, 1, 0b01000000); // Enable BDU
    I2Cdev::writeBytes(LIS3MDL_ADDR2, LSM6DSOX_CTRL7_G, 1, 0b10000000); // Enable High-Performance mode for Gyro

    I2Cdev::writeBytes(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG1, 1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
    I2Cdev::writeBytes(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG2, 1, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
    I2Cdev::writeBytes(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG3, 1, 0b00000000);// MD = 00 (continuous-conversion mode)
    I2Cdev::writeBytes(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG4, 1, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z) */ 
}

// Write to I2C device register
void I2C::writeI2C(int ADDR, int reg, int length, unsigned char* data)
{
    I2Cdev::writeBytes(ADDR, reg, length, data);
}

// Read from i2c device register
float I2C::readI2C(int ADDR, int reg, int length, int HandleI2C)
{
    if(HandleI2C == 3) // 24-bit read from Barometer
    {
        uint8_t buffer[3];
        I2Cdev::readBytes(ADDR, reg, length, buffer);
        I2CData_ = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2]; // Convertering af int til float
    }
    else if(HandleI2C == 2) // 16-bit read for Barometer calibration
    {
        uint8_t buff[2];
        I2Cdev::readBytes(ADDR, reg, length, buff);
        I2CData_ = buff[0]<<8 | buff[1]; // Convertering af int til float
    }
    else if(HandleI2C == 1) // 16-bit read from IMU
    {
        uint8_t buff[2]; // Read first register // Read second register
        uint8_t nbuff[2];
        I2Cdev::readBytes(ADDR, reg, length, buff);
        I2Cdev::readBytes(ADDR, reg+1, length, nbuff);
        if (buff ==-1 | nbuff ==-1) // Check read errors
        {
            printf("Failed to read from %d via I2C.", ADDR); // Read error
            exit(1);
        }
        int I2C_Data_ = (buff[0] | nbuff[0] << 8);

    }
    else // 8-bit read
    {
        uint8_t buff[1];
        I2Cdev::readBytes(ADDR, reg, length, buff);
        I2CData_ = buff[0]; // Convertering af int til float
    }

    return I2CData_; 
}