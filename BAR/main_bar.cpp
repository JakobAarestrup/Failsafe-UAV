//#include "I2C.hpp"
#include "BAR.hpp"
#include "MS5611.hpp"
#include <unistd.h>
#include <stdio.h>
#include "I2C_v2.hpp"

int main()
{
    BAR barometer;
    int calibration;

    barometer.calibrateBAR();

    while (true) 
    {
        barometer.readPressure();
        barometer.readTemperature();
        barometer.calculatePressureAndTemperature();
        
        calibration = barometer.getCalibration();
        if (calibration > 30)
        {
        printf("Height(m): %f\n",barometer.getHeight());
        sleep(1);
        }
        else
        {
            barometer.getHeight();
            printf("Calibrating barometer..\n");
            sleep(1);
        }
    }

    return 0;
}