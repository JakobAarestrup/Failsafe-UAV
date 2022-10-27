#include "BAR.hpp"
#include <math.h>

/*Math constants*/
#define p_0 101325
#define mbar_to_Pa 100
#define T_constant 44330
#define P_constant 1/5.255   

    // Constructor
    BAR::BAR() 
    {}

    // Destructor
    BAR::~BAR() 
    {
        /*delete [] height_AGL_, height_AMSL_, initial_pressure_, initial_AMSL_, pressure_; // Delete private variables*/
    }

    //Calibrates the barometer data
    void BAR::Calibrate_BAR(int x) 
    {
        initial_pressure_ = (x*mbar_to_Pa); // Convert mbar to Pascal
        initial_AMSL_ = T_constant*(1-(pow(initial_pressure_/p_0,P_constant)));//using international barometric formula to get height
    }

    // Converts the bar data into height
    void BAR::ConvertBARData(int x) 
    {
        pressure_ = (x*mbar_to_Pa); // Convert mbar to Pascal.
        height_AMSL_ = T_constant*(1-(pow(pressure_/p_0,P_constant))); // Using international barometric formula to get height
        height_AGL_ = height_AMSL_ - initial_AMSL_; // Subtract difference in height.
    }

    // Returns height above ground level
    int BAR::getHeight() const 
    {
        return height_AGL_; // Return height
    }