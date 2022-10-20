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

#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant
#define A_GAIN 0.0573    // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]
#define M_PI 3.14159265358979323846
#define RAD_TO_DEG 57.29578


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

int ReadI2C(int fd, int ADDR, int reg)
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
        //printf("Data from %d is: %d \n",ADDR, I2CData);
        return I2CData;
   }
}



int main()
{

// Open I2C connection
int fd = wiringPiI2CSetup(LSM6DSOX_ADDR2);
    if (fd ==-1)
    {
        printf("Failed to establish I2C connection.");
        exit(1);
    }
    printf("Succesfully setup I2C connection.\n");


// Enable accelerometer
WriteI2C(fd, LSM6DSOX_CTRL1_XL, 0b10100000);
WriteI2C(fd, LSM6DSOX_CTRL3_C, 0b01000000);

// Enable gyro
WriteI2C(fd, LSM6DSOX_CTRL2_G, 0b10101100);

while(1)
{
    int ACC_X {ReadI2C(fd, LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A)};
    int ACC_Y {ReadI2C(fd, LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A)};
    int ACC_Z {ReadI2C(fd, LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A)};

    // Convert accelerometer data
    double roll = 0.00, pitch = 0.00;
    double x_Buff = float(ACC_X);
    double y_Buff = float(ACC_Y);
    double z_Buff = float(ACC_Z);
    roll = atan2(y_Buff , z_Buff) * 57.3;
    pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
    printf("Roll: %f \n Pitch: %f \n", roll, pitch);

    int GYR_X {ReadI2C(fd, LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G)};
    int GYR_Y {ReadI2C(fd, LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G)};
    int GYR_Z {ReadI2C(fd, LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G)};

    //Convert Gyro raw to degrees per second
    float rate_gyr_y = 0.0;   // [deg/s]
	float rate_gyr_x = 0.0;   // [deg/s]
	float rate_gyr_z = 0.0;   // [deg/s]
	rate_gyr_x = (float) GYR_X  * G_GAIN;
	rate_gyr_y = (float) GYR_Y  * G_GAIN;
	rate_gyr_z = (float) GYR_Z  * G_GAIN;

    //Calculate the angles from the gyro
    float gyroXangle = 0.0;
	float gyroYangle = 0.0;
	float gyroZangle = 0.0;
	gyroXangle+=rate_gyr_x*DT;
	gyroYangle+=rate_gyr_y*DT;
	gyroZangle+=rate_gyr_z*DT;

    std::cout<<"GYR_X: "<<gyroXangle<<"\nGYR_Y: "<<gyroYangle<<"\nGYR_Z: "<<gyroZangle<<"\n";

    usleep(1000000);
}

return 0;
}