//#include "I2C.hpp"
#include "BAR.hpp"
#include "MS5611.hpp"
#include <unistd.h>
#include <stdio.h>
#include "I2C.hpp"

int main()
{
    BAR barometer;
    int calibration;

    barometer.calibrateBAR();

    for (int i = 0; i < 1031; i++)
    {
        barometer.readPressure();
        barometer.readTemperature();
        barometer.calculatePressureAndTemperature();

        barometer.getCalibration();
        /*    if (calibration > 30)
           {
           if (height < 0)
           {
           height  = 0;
           } */
        float height = barometer.getHeight();
        printf("%f\n", height);
        usleep(100000);
        // sleep(1);
    }
    /*    else
       {
           barometer.getHeight();
           printf("Calibrating barometer..\n");
           sleep(1);
       } */
}

return 0;
}