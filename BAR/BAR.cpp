#include "BAR.hpp"
#include "MS5611.hpp"
#include <math.h>
#include "I2Cdev.h"
#include <unistd.h>
#include <string>

/*Math constants*/
#define p_0 101325 // Pressure at sea level
#define mbar_to_Pa 100 // Conversion rate
#define T_s 288.15 // Temperature at sea level in Kelvin
#define R 287.052 // Specific gas constant in J/K*kg
#define T_G 0.0065 // Temperature gradient in K/m
#define g 9.807 // Gravitational constant in m/s^2

// Constructor
BAR::BAR() 
{}

// Destructor
BAR::~BAR() 
{
    /*delete [] height_AGL_, height_AMSL_, initial_pressure_, initial_AMSL_, pressure_; // Delete private variables*/
 }

//Calibrates the barometer data
void BAR::Calibrate_BAR() 
{
    initial_pressure_ = (PRES_*mbar_to_Pa); // Convert mbar to Pascal
    initial_AMSL_ = (T_s/T_G)*(1-pow((initial_pressure_/p_0),T_G*(R/g)))+deviation_; //using international barometric formula to get height
    //printf("Initial AMSL: %f\n", initial_AMSL_);
}

// Converts the bar data into height
void BAR::ConvertBARData() 
{
    pressure_ = (PRES_*mbar_to_Pa); // Convert mbar to Pascal.
    
    height_AMSL_ = (T_s/T_G)*(1-pow((pressure_/p_0),T_G*(R/g)))+deviation_; // Using international barometric formula to get height
    //printf("Height AMSL: %f\n", height_AMSL_);
    height_AGL_ = height_AMSL_ - initial_AMSL_; // Subtract difference in height.
}

// Returns height above ground level
float BAR::getHeight() 
{
    
    if(calibration_ <= 30)
    {
        Calibrate_BAR();
        calibration_++;
    }
        ConvertBARData();
        return height_AGL_; // Return height
}

int BAR::getCalibration()
{
    return calibration_;
}

// Power on and prepare for general usage. This method reads coefficients stored in PROM.
void BAR::initialize() 
{
    // Reading 6 calibration data values
    uint8_t buff[2];
    I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_C1, 2, buff);
    C1_ = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_C2, 2, buff);
    C2_ = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_C3, 2, buff);
    C3_ = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_C4, 2, buff);
    C4_ = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_C5, 2, buff);
    C5_ = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_C6, 2, buff);
    C6_ = buff[0]<<8 | buff[1];

    update();
}

// Verify the I2C connectio. Returns True if connection is valid, false otherwise
bool BAR::testConnection() 
{
    uint8_t data;
    int8_t status = I2Cdev::readByte(MS5611_DEFAULT_ADDRESS, MS5611_RA_C0, &data);
    if (status > 0)
        return true;
    else
        return false;
}

// Initiate the process of pressure measurement
void BAR::refreshPressure() 
{
    I2Cdev::writeBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_D1_OSR_4096, 0, 0);
}

// Read pressure value
void BAR::readPressure() 
{
    uint8_t buffer[3];
    I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_ADC, 3, buffer);
    D1_ = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
}

// Initiate the process of temperature measurement
void BAR::refreshTemperature() 
{
	I2Cdev::writeBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_D2_OSR_4096, 0, 0);
}

// Read temperature value
void BAR::readTemperature() 
{
	uint8_t buffer[3];
	I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_ADC, 3, buffer);
	D2_ = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
}

// Calculate temperature and pressure calculations and perform compensation. More info about these calculations is available in the datasheet.
void BAR::calculatePressureAndTemperature() 
{
    float dT = D2_ - C5_ * pow(2, 8);
    TEMP_ = (2000 + ((dT * C6_) / pow(2, 23)));
    float OFF = C2_ * pow(2, 16) + (C4_ * dT) / pow(2, 7);
    float SENS = C1_ * pow(2, 15) + (C3_ * dT) / pow(2, 8);

    float T2, OFF2, SENS2;

    if (TEMP_ >= 2000)
    {
        T2 = 0;
        OFF2 = 0;
        SENS2 = 0;
    }
    if (TEMP_ < 2000)
    {
        T2 = dT * dT / pow(2, 31);
        OFF2 = 5 * pow(TEMP_ - 2000, 2) / 2;
        SENS2 = OFF2 / 2;
    }
    if (TEMP_ < -1500)
    {
        OFF2 = OFF2 + 7 * pow(TEMP_ + 1500, 2);
        SENS2 = SENS2 + 11 * pow(TEMP_ + 1500, 2) / 2;
    }

    TEMP_ = TEMP_ - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    // Final calculations
    PRES_ = ((D1_ * SENS) / pow(2, 21) - OFF) / pow(2, 15) / 100;
    TEMP_ = TEMP_ / 100;
}

/** Perform pressure and temperature reading and calculation at once.
 *  Contains sleeps, better perform operations separately.
 */
void BAR::update() 
{
    refreshPressure();
    usleep(10000); // Waiting for pressure data ready
    readPressure();

    refreshTemperature();
    usleep(10000); // Waiting for temperature data ready
    readTemperature();

    calculatePressureAndTemperature();
}

// Get calculated temperature value. Return Temperature in degrees of Celsius
float BAR::getTemperature() 
{
    return TEMP_;
}

// Get calculated pressure value. Return Pressure in millibars
float BAR::getPressure() 
{
	return PRES_;
}