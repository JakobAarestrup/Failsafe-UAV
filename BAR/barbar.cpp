#include "I2C.hpp"
#include "BAR.hpp"
#include "MS5611.hpp"
#include <unistd.h>
#include <stdio.h>

int main()
{
    BAR barometer;
    barometer.calibrateBAR();
    while (1)
    {
        barometer.getHeight();
        usleep(100000);
    }

    return 0;
}