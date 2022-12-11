#include "I2C.hpp"
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

/**
 * @brief Construct a new I2C::I2C object
 *
 */
I2C::I2C()
{
}

/**
 * @brief Destroy the I2C::I2C object
 *
 */
I2C::~I2C()
{
}

/**
 * @brief Function initializes the I2C communication and sets registers of accelerometer, gyroscope, magnetometer and the free fall functionality
 *
 */
void I2C::initializeI2C()
{
    // First IMU:
    I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL2_G, 0b01001000);   // Gyro = 208 Hz (normal mode) 1000 dps
    I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL1_XL, 0b011000000); // Acc = 416 Hz (High-Performance mode) 2g
    I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL3_C, 0b01000000);   // Enable BDU
    I2Cdev::writeByte(LSM6DSOX_ADDR1, LSM6DSOX_CTRL7_G, 0b01000000);   // enable HPF // default HPF (00) 16 Hz
                                                                       // I2Cdev::writeByte(LIS3MDL_ADDR1, LSM6DSOX_CTRL7_G, 0b10000000); // Enable High-Performance mode for Gyro

    // Enable magnetometer from IMU1
    I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG1, 0b01100010); // OM = 11 (ultra-high-performance mode for X and Y) FAST_ODR = 1 (155Hz ODR)
    I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG2, 0b00000000); // FS = 00 (+/- 4 gauss full scale) Default
    I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG3, 0b00000000); // MD = 00 (continuous-conversion mode)
    I2Cdev::writeByte(LIS3MDL_ADDR1, LIS3MDL_CTRL_REG4, 0b00001100); // OMZ = 11 (ultra-high-performance mode for Z)

    // Second IMU:
    // I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
    /*  I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL2_G, 0b01001000);   // Gyro = 208 Hz (normal mode) 1000 dps
     I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL1_XL, 0b011000000); // Acc = 416 Hz (High-Performance mode) 2g
     I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000);   // Enable BDU
     I2Cdev::writeByte(LSM6DSOX_ADDR2, LSM6DSOX_CTRL7_G, 0b01000000);   // enable HPF // default HPF (00) 16 Hz
                                                                        // I2Cdev::writeByte(LIS3MDL_ADDR2, LSM6DSOX_CTRL7_G, 0b10000000); // Enable High-Performance mode for Gyro

     I2Cdev::writeByte(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG1, 0b01110000); // OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
     I2Cdev::writeByte(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG2, 0b00000000); // FS = 00 (+/- 4 gauss full scale)
     I2Cdev::writeByte(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG3, 0b00000000); // MD = 00 (continuous-conversion mode)
     I2Cdev::writeByte(LIS3MDL_ADDR2, LIS3MDL_CTRL_REG4, 0b00001100); // OMZ = 11 (ultra-high-performance mode for Z)
}

/**
 * @brief Function writes to I2C device register
 *
 * @param ADDR is the I2C address of the device
 * @param reg  is the register to be written to
 * @param length is the length of the data sent
 * @param data is the data that is written to the I2C device
 */
    void I2C::writeI2C(int ADDR, int reg, int length, unsigned char *data)
    {
        I2Cdev::writeBytes(ADDR, reg, length, data);
    }

    /**
     * @brief Function reads from I2C device
     *
     * @param ADDR is the I2C address of the device
     * @param reg  is the register to be read from
     * @param length is the length of the data to be read
     * @param HandleI2C is the handler for what type of register that is read from
     * @return float dataI2C_
     */
    float I2C::readI2C(int ADDR, int reg, int length, int HandleI2C)
    {
        if (HandleI2C == 3) // 24-bit read from Barometer
        {
            uint8_t buff[3];
            I2Cdev::readBytes(ADDR, reg, length, buff);
            dataI2C_ = (buff[0] << 16) | (buff[1] << 8) | buff[2]; // Conversion of int to float
        }
        else if (HandleI2C == 2) // 16-bit read for Barometer calibration
        {
            uint8_t buff[2];
            I2Cdev::readBytes(ADDR, reg, length, buff);
            dataI2C_ = buff[0] << 8 | buff[1]; // Conversion of int to float
        }
        else if (HandleI2C == 1) // 16-bit read from IMU
        {
            uint8_t buff; // Read first register // Read second register
            uint8_t nbuff;
            I2Cdev::readBytes(ADDR, reg, length, &buff);
            I2Cdev::readBytes(ADDR, reg + 1, length, &nbuff);
            dataI2C_ = (buff | nbuff << 8);

            if (dataI2C_ < 0x8000) // two's complement read from 16 bit register
            {
            }
            else
            {
                dataI2C_ = dataI2C_ - 0xFFFF;
            }
        }
        else // 8-bit read
        {
            uint8_t buff;
            I2Cdev::readBytes(ADDR, reg, length, &buff);
            dataI2C_ = buff; // Conversion of int to float
        }
        return dataI2C_;
    }