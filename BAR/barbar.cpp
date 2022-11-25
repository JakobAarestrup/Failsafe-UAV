#include "I2C.hpp"
#include "BAR.hpp"
#include "MS5611.hpp"
#include <unistd.h>
#include <stdio.h>

int main()
{
    BAR barometer;
    barometer.calibrateBAR();
    float height;

    while (1)
    {
        barometer.update();
        height = barometer.getHeight();
        printf("%f\n", height);
        usleep(100000);
    }

    return 0;
}