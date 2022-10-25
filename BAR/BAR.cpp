#include "BAR.hpp"
#include <math.h>

#define KPa 100
#define mbar_to_bar 1000
#define constant 1
    
    BAR::BAR()
    {}

    BAR::~BAR()
    {}


    void BAR::ConvertBARData(int x)
    {
        pressure_ = (x/mbar_to_bar)*KPa;
        height_AMSL_ = pressure_;
        height_AGL_ = height_AMSL_ + AGL_AMSL_diff_;

    }


    int BAR::getHeight()
    {
        return height_;
    }