#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "I2C.hpp"
#include "IMU.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "/Fusion-main/Fusion/Fusion.h"
#include <stdbool.h>
#include <time.h>

#define SAMPLE_RATE (100) // replace this with actual sample rate

int main()
{

I2C I1;
I2C I2;
IMU IMU2;
FusionAhrs ahrs;
int accData[3];
int gyrData[3];
int magData[3];

// Open I2C connection
I1.openI2C(LSM6DSOX_ADDR2);
I2.openI2C(LIS3MDL_ADDR_2);

// Enable accelerometer
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL1_XL, 0b10100000);
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000);

// Enable gyro
I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL2_G, 0b10101100);

// Enable magnetometer - 
I2.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG1, 0b01110000);// OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
I2.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG2, 0b00000000);// FS = 00 (+/- 4 gauss full scale)
I2.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG3, 0b00000000);// MD = 00 (continuous-conversion mode)
I2.WriteI2C(LIS3MDL_ADDR_2, LIS3MDL_CTRL_REG4, 0b00001100);// OMZ = 11 (ultra-high-performance mode for Z)

// Main loop
while(1)
{
    int ax = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A);
    int ay = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A);
    int az = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A);
    int gx = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G);
    int gy = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G);
    int gz = I1.ReadI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G);
    int mx = I2.ReadI2C(LIS3MDL_ADDR_2, LIS3MDL_OUT_X_L);
    int my = I2.ReadI2C(LIS3MDL_ADDR_2, LIS3MDL_OUT_Y_L);
    int mz = I2.ReadI2C(LIS3MDL_ADDR_2, LIS3MDL_OUT_Z_L);

    //IMU2.CreateIMUData(ax, ay, az,gx, gy, gz, mx, my, mz);
    //accData = IMU2.getACCData();
    //gyrData = IMU2.getGYRData();
    //magData = IMU2.getMAGData();

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
    const FusionAhrsSettings settings = {
            .gain = 0.5f,
            .accelerationRejection = 10.0f,
            .magneticRejection = 20.0f,
            .rejectionTimeout = 5 * SAMPLE_RATE, /* 5 seconds */
    };
    FusionAhrsSetSettings(&ahrs, &settings);

    // This loop should repeat each time new gyroscope data is available
    while (true) {

        // Acquire latest sensor data
        const clock_t timestamp = clock(); // replace this with actual gyroscope timestamp
        FusionVector gyroscope = {0.0f, 0.0f, 0.0f}; // replace this with actual gyroscope data in degrees/s
        FusionVector accelerometer = {0.0f, 0.0f, 1.0f}; // replace this with actual accelerometer data in g
        FusionVector magnetometer = {1.0f, 0.0f, 0.0f}; // replace this with actual magnetometer data in arbitrary units

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

    //printf("Roll: %f    Pitch: %f \n", roll, pitch); //Print accelerometer data
    //printf("GYR_X: %f   GYR_Y: %f   GYR_Z: %f \n", GYR_X, GYR_Y, GYR_Z); //Print gyroscope data
    //printf("MAG_x: %d \nMAG_y: %d \nMAG_z: %d \n", MAG_x, MAG_y, MAG_z); //Print mag data
    //printf("MAG_X: %d       MAG_Y: %d       MAG_Z: %d \n", MAG_X, MAG_Y, MAG_Z); //Print mag data
    
    //usleep(1000000); //Small delay

    return 0;
}