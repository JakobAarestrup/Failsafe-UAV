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
    void convertBARData(); // Converts the bar data into height
    void calibrate_BAR(); // Calibrates the barometer data
    void initialize();
    bool testConnection();
    void refreshPressure();
    void readPressure();
    void refreshTemperature();
    void readTemperature();
    void calculatePressureAndTemperature();
    void update();
    //float getTemperature();
    //float getPressure();
    float  getHeight(); // Returns the height above ground level
    int getCalibration();
private:
    float initial_AMSL_; // Initial height above mean sea level when drone is on the ground
    float initial_pressure_;  // Initial pressure when drone is on the ground
    float height_AMSL_; // Height above mean sea level
    float height_AGL_; // Height above ground level
    float pressure_; // Pressure from barometer
    uint16_t C1_, C2_, C3_, C4_, C5_, C6_; // Calibration data
    uint32_t D1_, D2_; // Raw measurement data
    float temp_; // Calculated temperature
	float pres_; // Calculated pressure
    float deviation_ = -48.79; // Measured deviation
    int calibration_ = 0; // Calibration variable for getHeight()
    int BAR_I2C_;
};