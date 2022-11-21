#include <math.h>
#include <cstdio>

class BAR_UNIT
{
public:
    //float BAR_UNIT::calculatePressureAndTemperature(float temp, float pres, int handle);
    float initialAMSL(float pres);
    float convertToAGL(float pres);
private:
    float initial_AMSL_; // Initial height above mean sea level when drone is on the ground
    float height_AMSL_; // Height above mean sea level
    float height_AGL_; // Height above ground level
    //uint32_t D1_, D2_; // Raw measurement data
    //uint16_t C1_, C2_, C3_, C4_, C5_, C6_; // Calibration data
    float temp_; // Calculated temperature
	float pres_; // Calculated pressure
    //int calibration_ = 0; // Calibration variable for getHeight()
};