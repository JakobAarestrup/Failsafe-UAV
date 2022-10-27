#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "I2C.hpp"
#include "BAR.hpp"
#include "MS5611.hpp"

int main()
{

I2C I1;
BAR B1;

// Open I2C connection
I1.openI2C(MS5611_ADDRESS);

// Enable Barometer
//I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL1_XL, 0b10100000);
//I1.WriteI2C(LSM6DSOX_ADDR2, LSM6DSOX_CTRL3_C, 0b01000000);

// Calibrate Barometer
int BAR_data = I1.ReadI2C_8bit(MS5611_ADDRESS, PROM_READ);
B1.Calibrate_BAR(BAR_data);

// Main loop
while(1)
{

    BAR_data = I1.ReadI2C_8bit(MS5611_ADDRESS, PROM_READ);
    printf("Raw Barometer Data: %d\n", BAR_data);
    B1.ConvertBARData(BAR_data);

    int height = B1.getHeight();
    printf("Height: %d\n", height);
    
    usleep(1000000); //Small delay
}

return 0;
}