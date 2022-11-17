#include "BAR_UNIT.cpp"

int main()
{
    BAR_UNIT b1;

    float temp1 = 1;
    float temp2 = 2;
    float temp3 = 3;
    float pres1  = 1;
    float pres2  = 2;
    float pres3  = 3;

    b1.calculatePressureAndTemperature(temp1,pres1);
    b1.initialAMSL(pres1);
    b1.convertToAGL(pres1);

    return 0;
}