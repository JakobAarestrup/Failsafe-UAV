#include "BAR.hpp"
#include <math.h>

#define p_0 101325
#define mbar_to_Pa 100
#define T_constant 44330
#define P_constant 1/5.255
    
    BAR::BAR()
    {}

    BAR::~BAR()
    {
        delete [] height_AGL_, height_AMSL_, initial_pressure_, pressure_
    }

    void BAR::Calibrate_BAR(int x)
    {
        initial_pressure_ = (x*mbar_to_Pa); // convert mbar to Pascal.
        initial_AMSL_ = T_constant*(1-(initial_pressure_/p_0)^P_constant);//using internation barometric formula.
    }

    void BAR::ConvertBARData(int x)
    {
        pressure_ = (x*mbar_to_Pa); // convert mbar to Pascal.
        height_AMSL_ = T_constant*(1-(pressure_/p_0)^P_constant); //using internation barometric formula.
        height_AGL_ = height_AMSL_ - initial_AMSL_; // subtract difference in height.
    }

    int BAR::getHeight() const
    {
        return height_AGL_; //return height
    }