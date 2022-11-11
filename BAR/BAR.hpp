#include <stdint.h>
#include "MS5611.hpp"
#include <math.h>
#include "I2Cdev.h"
#include <unistd.h>
#include <string>

class BAR
{
public:
    BAR(); // Constructor
    ~BAR();// Destructor
    void ConvertToAGL(); // Converts the bar data into height in AGL
    void InitialAMSL(); // Collects the initial AMSL value
    void CalibrateBAR(); // Calibrates the barometer
    int GetCalibration(); // Returns the calibration variable
    bool TestConnection();
    //void RefreshPressure();
    void ReadPressure();
    //void RefreshTemperature();
    void ReadTemperature();
    void CalculatePressureAndTemperature();
    void Update();
    float GetHeight(); // Returns the height above ground level
private:
    float initial_AMSL_; // Initial height above mean sea level when drone is on the ground
    float height_AMSL_; // Height above mean sea level
    float height_AGL_; // Height above ground level
    uint16_t C1_, C2_, C3_, C4_, C5_, C6_; // Calibration data
    uint32_t D1_, D2_; // Raw measurement data
    float temp_; // Calculated temperature
	float pres_; // Calculated pressure
    int calibration_ = 0; // Calibration variable for getHeight()
};