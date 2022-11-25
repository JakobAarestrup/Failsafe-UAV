//#include "I2C.hpp"
#include "BAR.hpp"
#include "MS5611.hpp"
#include <unistd.h>
#include <stdio.h>

int main()
{
    BAR barometer;
    int calibration;
    while (1)
    {
        barometer.calibrateBAR();
        barometer.getHeight();
        usleep(100000);
    }

    return 0;
}