#include "I2C.hpp"
#include "BAR.hpp"
#include "MS5611.hpp"
#include <unistd.h>
#include <stdio.h>

int main()
{
    BAR barometer;

    barometer.initialize();

    while (true) 
    {
        barometer.refreshPressure();
        usleep(10000); // Waiting for pressure data ready
        barometer.readPressure();

        barometer.refreshTemperature();
        usleep(10000); // Waiting for temperature data ready
        barometer.readTemperature();

        barometer.calculatePressureAndTemperature();
        
        //printf("Temperature(C): %f Pressure(mbar): %f Height(m): %f\n", barometer.getTemperature(), barometer.getPressure(), barometer.getHeight());
        printf("Height(m): %f\n",barometer.getHeight());
        sleep(1);
    }

    return 0;
}