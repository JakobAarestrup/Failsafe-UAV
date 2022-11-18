#included <math.h>

class BAR_UNIT
{
public:
    float BAR_UNIT::calculatePressureAndTemperature(float temp, float pres, int handle);
    float BAR_UNIT::initialAMSL(float pres);
    float BAR_UNIT::convertToAGL(float pres);
private:
    float initial_AMSL_; // Initial height above mean sea level when drone is on the ground
    float height_AMSL_; // Height above mean sea level
    float height_AGL_; // Height above ground level
    uint32_t D1_, D2_; // Raw measurement data
    uint16_t C1_, C2_, C3_, C4_, C5_, C6_; // Calibration data
    float temp_; // Calculated temperature
	float pres_; // Calculated pressure
    int calibration_ = 0; // Calibration variable for getHeight()
}

/*Math constants*/
#define p_0 101325 // Pressure at sea level
#define mbar_to_Pa 100 // Conversion rate
#define T_s 288.15 // Temperature at sea level in Kelvin
#define R 287.052 // Specific gas constant in J/K*kg
#define T_G 0.0065 // Temperature gradient in K/m
#define g 9.807 // Gravitational constant in m/s^2

float BAR_UNIT::calculatePressureAndTemperature(float temp, float pres, int handle) 
{
    float dT = D2_ - C5_ * pow(2, 8);
    temp_ = (2000 + ((dT * C6_) / pow(2, 23)));
    float OFF = C2_ * pow(2, 16) + (C4_ * dT) / pow(2, 7);
    float SENS = C1_ * pow(2, 15) + (C3_ * dT) / pow(2, 8);

    float T2, OFF2, SENS2;

    if (temp_ >= 2000)
    {
        T2 = 0;
        OFF2 = 0;
        SENS2 = 0;
    }
    if (temp_ < 2000)
    {
        T2 = dT * dT / pow(2, 31);
        OFF2 = 5 * pow(temp_ - 2000, 2) / 2;
        SENS2 = OFF2 / 2;
    }
    if (temp_ < -1500)
    {        OFF2 = OFF2 + 7 * pow(temp_ + 1500, 2);
        SENS2 = SENS2 + 11 * pow(temp_ + 1500, 2) / 2;
    }

    temp_ = temp_ - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    // Final calculations
    pres_ = (((D1_ * SENS) / pow(2, 21) - OFF) / pow(2, 15) / 100)*mbar_to_Pa;
    temp_ = temp_ / 100;

    if (handle == 1)
    {
        return temp_;
    }
    else
    {
        return pres_;
    }
}

//Calibrates the barometer data
float BAR_UNIT::initialAMSL(float pres) 
{
    initial_AMSL_ = (T_s/T_G)*(1-pow((pres_/p_0),T_G*(R/g))); //using international barometric formula to get height
    return initial_AMSL_;
}

// Converts the bar data into height
float BAR_UNIT::convertToAGL(float pres) 
{    
    height_AMSL_ = (T_s/T_G)*(1-pow((pres_/p_0),T_G*(R/g))); // Using international barometric formula to get height
    height_AGL_ = height_AMSL_ - initial_AMSL_; // Subtract difference in height.
    return height_AGL_;
}