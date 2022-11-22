#include "BAR_UNIT.hpp"

/*Math constants*/
#define p_0 101325 // Pressure at sea level
#define mbar_to_Pa 100 // Conversion rate
#define T_s 288.15 // Temperature at sea level in Kelvin
#define R 287.052 // Specific gas constant in J/K*kg
#define T_G 0.0065 // Temperature gradient in K/m
#define g 9.807 // Gravitational constant in m/s^2

//Calibrates the barometer data
float BAR_UNIT::initialAMSL(float pres) 
{
    initial_AMSL_ = (T_s/T_G)*(1-pow((pres/p_0),T_G*(R/g))); //using international barometric formula to get height
    return initial_AMSL_;
}

// Converts the bar data into height
float BAR_UNIT::convertToAGL(float pres) 
{    
    height_AMSL_ = (T_s/T_G)*(1-pow((pres/p_0),T_G*(R/g))); // Using international barometric formula to get height
    height_AGL_ = height_AMSL_ - initial_AMSL_; // Subtract difference in height.
    return height_AGL_;
}