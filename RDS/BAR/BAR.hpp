#pragma once
#include <stdint.h>
#include "MS5611.hpp"
#include <math.h>
#include "../IMU/I2C.hpp"
#include <unistd.h>
#include <string>

class BAR
{
public:
    BAR() : {}
    ~BAR();
    void convertToAGL();
    void initialAMSL();
    void calibrateBAR();
    int getCalibration();
    void readPressure();
    void readTemperature();
    void calculatePressureAndTemperature();
    void update();
    float getHeight();

private:
    /// Initial height above mean sea level when drone is on the ground
    float initial_AMSL_;
    /// Height above mean sea level
    float height_AMSL_;
    /// Height above ground level
    float height_AGL_;
    /// Raw measurement data
    uint32_t D1_, D2_;
    /// Calibration data
    uint16_t C1_, C2_, C3_, C4_, C5_, C6_;
    /// Calculated temperature
    float temp_;
    /// Calculated pressure
    float pres_;
    /// Calibration variable for getHeight()
    int calibration_;
};