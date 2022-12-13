#include "BAR.hpp"

/**
 * Math constants used for barometric formula
 */
/// Pressure at sea level
#define p_0 101325
/// Conversion rate
#define mbar_to_Pa 100
/// Temperature at sea level in Kelvin
#define T_s 288.15
/// Specific gas constant in J/K*kg
#define R 287.052
/// Temperature gradient in K/m
#define T_G 0.0065
/// Gravitational constant in m/s^2
#define g 9.807

/// I2C class member
I2C B1;

/**
 * @brief Construct a new BAR::BAR object
 *
 */
BAR::BAR()
{
}

/**
 * @brief Destroy the BAR::BAR object
 *
 */
BAR::~BAR()
{
}

/**
 * @brief Function calculates the initial above mean sea level altitude
 *
 */
void BAR::initialAMSL()
{
    initial_AMSL_ = (T_s / T_G) * (1 - pow((pres_ / p_0), T_G * (R / g))); // Using barometric formula.
    if (initial_AMSL_ < 0)
    {
        initial_AMSL_ = 0;
    }
}

/**
 * @brief Function converts the bar data into height above ground level
 *
 */
void BAR::convertToAGL()
{
    height_AMSL_ = (T_s / T_G) * (1 - pow((pres_ / p_0), T_G * (R / g))); // Using barometric formula.
    height_AGL_ = height_AMSL_ - initial_AMSL_;                           // Subtract difference in height.
    if (height_AGL_ < 0)
    {
        height_AGL_ = 0;
    }
}

/**
 * @brief Function returns height above ground level
 *
 * @return float height_AGL_
 */
float BAR::getHeight()
{
    convertToAGL();
    return height_AGL_; // Return height
}

/**
 * @brief Function returns BAR calibration private variable
 *
 * @return int calibration_
 */
int BAR::getCalibration()
{
    return calibration_;
}

/**
 * @brief Function prepares for general usage. This method reads coefficients stored in PROM and calibrates the Barometer
 *
 */
void BAR::calibrateBAR()
{

    for (int i = 0; i < 31; i++)
    {
        if (calibration_ == 0)
        {
            // Reading 6 calibration data values
            C1_ = B1.readI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C1, 2, 2);
            C2_ = B1.readI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C2, 2, 2);
            C3_ = B1.readI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C3, 2, 2);
            C4_ = B1.readI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C4, 2, 2);
            C5_ = B1.readI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C5, 2, 2);
            C6_ = B1.readI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C6, 2, 2);
            calibration_++;
        }
        else
        {
            update();
            initialAMSL();
            usleep(1000000);
            printf("Calibrating baro...%d\n", i);
        }
    }
}

/**
 * @brief Function that reads the pressure value from the barometer
 *
 */
void BAR::readPressure()
{
    // Initiate the process of pressure measurement
    B1.writeI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_D1_OSR_4096, 0, 0);
    usleep(10000);
    // Read pressure value
    D1_ = B1.readI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_ADC, 3, 3);
}

/**
 * @brief Function reads the temperature value from the barometer
 *
 */
void BAR::readTemperature()
{
    // Initiate the process of temperature measurement
    B1.writeI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_D2_OSR_4096, 0, 0);
    usleep(10000);
    // Read temperature value
    D2_ = B1.readI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_ADC, 3, 3);
}

/**
 * @brief Function does temperature and pressure calculations and perform compensation. More info about these calculations is available in the datasheet.
 *
 */
void BAR::calculatePressureAndTemperature()
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
    {
        OFF2 = OFF2 + 7 * pow(temp_ + 1500, 2);
        SENS2 = SENS2 + 11 * pow(temp_ + 1500, 2) / 2;
    }

    temp_ = temp_ - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    // Final calculations
    pres_ = (((D1_ * SENS) / pow(2, 21) - OFF) / pow(2, 15) / 100) * mbar_to_Pa;
    temp_ = temp_ / 100;
}

/**
 * @brief Function updates the pressure and temperature and does compensation by calling other functions in BAR
 *
 */
void BAR::update()
{
    readPressure();
    readTemperature();
    calculatePressureAndTemperature();
}