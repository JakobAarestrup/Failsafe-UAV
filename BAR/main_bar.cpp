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

    barometer.CalibrateBAR();

    while (true) 
    {
        //arometer.refreshPressure();
        //usleep(10000); // Waiting for pressure data ready
        barometer.readPressure();

        //barometer.refreshTemperature();
        //usleep(10000); // Waiting for temperature data ready
        barometer.readTemperature();

        barometer.calculatePressureAndTemperature();
        
        //printf("Temperature(C): %f Pressure(mbar): %f Height(m): %f\n", barometer.getTemperature(), barometer.getPressure(), barometer.getHeight());
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