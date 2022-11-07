#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "I2C.hpp"
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <time.h>
#include "Fusion.h"

#define SAMPLE_RATE (2000) // replace this with actual sample rate

int main()
{
I2C I1;
I2C I2;
IMU IMU1;
float T_old = 0;
float dT = 2000;

// Open I2C connection
I1.openI2C(LSM6DSOX_ADDR1);
I2.openI2C(LIS3MDL_ADDR_1);
float T = millis();

// Enable accelerometer and gyroscope
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_INT1_CTRL, 0b00000011); // Enable gyroscope and accelerometer data interrupt
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL2_G, 0b10011100); // Gyro = 416 Hz (High-Performance mode)
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL1_XL, 0b011000000); // Acc = 416 Hz (High-Performance mode)
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL3_C, 0b01000000); // Enable BDU
I1.WriteI2C(LSM6DSOX_ADDR1, LSM6DSOX_CTRL7_G, 0b10000000); // Enable High-Performance mode for Gyro

// Enable magnetometer
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2.WriteI2C(LIS3MDL_ADDR_1, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)

// Define calibration (replace with actual calibration data)
    const FusionMatrix gyroscopeMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    const FusionVector gyroscopeSensitivity = {1.0f, 1.0f, 1.0f};
    const FusionVector gyroscopeOffset = {0.0f, 0.0f, 0.0f};
    const FusionMatrix accelerometerMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    const FusionVector accelerometerSensitivity = {1.0f, 1.0f, 1.0f};
    const FusionVector accelerometerOffset = {0.0f, 0.0f, 0.0f};
    const FusionMatrix softIronMatrix = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    const FusionVector hardIronOffset = {0.0f, 0.0f, 0.0f};

    // Initialise algorithms
    FusionOffset offset;
    FusionAhrs ahrs;

    FusionOffsetInitialise(&offset, SAMPLE_RATE);
    FusionAhrsInitialise(&ahrs);

    // Set AHRS algorithm settings
    const FusionAhrsSettings settings = {0.5f, 10.0f, 20.0f, 5 * SAMPLE_RATE /* 5 seconds */};
    FusionAhrsSetSettings(&ahrs, &settings);

    // Main loop
    while (true) 
    {
        // Acquire latest sensor data
        float ax = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A);
        float ay = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A);
        float az = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A);
        float gx = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G);
        float gy = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G);
        float gz = I1.ReadI2C_16bit(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G);
        float mx = I2.ReadI2C_16bit(LIS3MDL_ADDR_2, LIS3MDL_OUT_X_L);
        float my = I2.ReadI2C_16bit(LIS3MDL_ADDR_2, LIS3MDL_OUT_Y_L);
        float mz = I2.ReadI2C_16bit(LIS3MDL_ADDR_2, LIS3MDL_OUT_Z_L);
        const clock_t timestamp = clock(); // replace this with actual gyroscope timestamp
        FusionVector accelerometer = {ax, ay, az}; // replace this with actual accelerometer data in g
        FusionVector gyroscope = {gx, gy, gz}; // replace this with actual gyroscope data in degrees/s
        FusionVector magnetometer = {mx, my, mz}; // replace this with actual magnetometer data in arbitrary units

        // Apply calibration
        gyroscope = FusionCalibrationInertial(gyroscope, gyroscopeMisalignment, gyroscopeSensitivity, gyroscopeOffset);
        accelerometer = FusionCalibrationInertial(accelerometer, accelerometerMisalignment, accelerometerSensitivity, accelerometerOffset);
        magnetometer = FusionCalibrationMagnetic(magnetometer, softIronMatrix, hardIronOffset);

        // Update gyroscope offset correction algorithm
        gyroscope = FusionOffsetUpdate(&offset, gyroscope);

        // Calculate delta time (in seconds) to account for gyroscope sample clock error
        static clock_t previousTimestamp;
        const float deltaTime = (float) (timestamp - previousTimestamp) / (float) CLOCKS_PER_SEC;
        previousTimestamp = timestamp;

        // Update gyroscope AHRS algorithm
        FusionAhrsUpdate(&ahrs, gyroscope, accelerometer, magnetometer, deltaTime);

        // Print algorithm outputs
        const FusionEuler euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&ahrs));
        const FusionVector earth = FusionAhrsGetEarthAcceleration(&ahrs);

        printf("Roll %0.1f, Pitch %0.1f, Yaw %0.1f, X %0.1f, Y %0.1f, Z %0.1f\n",
               euler.angle.roll, euler.angle.pitch, euler.angle.yaw,
               earth.axis.x, earth.axis.y, earth.axis.z);
    }

return 0;
}