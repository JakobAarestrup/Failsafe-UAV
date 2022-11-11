#include "BAR.hpp"

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
void BAR::InitialAMSL() 
{
    initial_AMSL_ = (T_s/T_G)*(1-pow((pres_/p_0),T_G*(R/g))); //using international barometric formula to get height
}

// Converts the bar data into height
void BAR::ConvertToAGL() 
{    
    height_AMSL_ = (T_s/T_G)*(1-pow((pres_/p_0),T_G*(R/g))); // Using international barometric formula to get height
    height_AGL_ = height_AMSL_ - initial_AMSL_; // Subtract difference in height.
}

// Returns height above ground level
float BAR::GetHeight() 
{    
    if(calibration_ <= 29)
    {
        InitialAMSL();
        calibration_++;
    }
        ConvertToAGL();
        return height_AGL_; // Return height
}

int BAR::GetCalibration()
{
    return calibration_;
}

// Power on and prepare for general usage. This method reads coefficients stored in PROM.
void BAR::CalibrateBAR() 
{
    // Reading 6 calibration data values
    uint8_t buff[2];
    I2C::ReadI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C1, 2, buff);
    C1_ = buff[0]<<8 | buff[1];
    I2C::ReadI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C2, 2, buff);
    C2_ = buff[0]<<8 | buff[1];
    I2C::ReadI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C3, 2, buff);
    C3_ = buff[0]<<8 | buff[1];
    I2C::ReadI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C4, 2, buff);
    C4_ = buff[0]<<8 | buff[1];
    I2C::ReadI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C5, 2, buff);
    C5_ = buff[0]<<8 | buff[1];
    I2C::ReadI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C6, 2, buff);
    C6_ = buff[0]<<8 | buff[1];

    Update();
}

// Verify the I2C connection. Returns True if connection is valid, false otherwise
bool BAR::TestConnection() 
{
    uint8_t data;
    int8_t status = I2C::ReadI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_C0, &data);
    if (status > 0)
        return true;
    else
        return false;
}

/*
// Initiate the process of pressure measurement
void BAR::RefreshPressure() 
{
    I2C::WriteI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_D1_OSR_4096, 0, 0);
}
*/

// Read pressure value
void BAR::ReadPressure() 
{
    
    // Initiate the process of pressure measurement
    I2C::WriteI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_D1_OSR_4096, 0, 0);
    usleep(10000); // Waiting for pressure data ready

    // Read pressure value
    //uint8_t buffer[3];
    D1_ = I2C::ReadI2C(MS5611_DEFAULT_ADDRESS, MS5611_RA_ADC, 3);
    //D1_ = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
}

/*
// Initiate the process of temperature measurement
void BAR::RefreshTemperature() 
{
	I2Cdev::writeBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_D2_OSR_4096, 0, 0);
}
*/

// Read temperature value
void BAR::ReadTemperature() 
{
    // Initiate the process of temperature measurement
    I2Cdev::writeBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_D2_OSR_4096, 0, 0);
    usleep(10000); // Waiting for temperature data ready

    // Read temperature value
    //uint8_t buffer[3];
	D2_ = I2Cdev::readBytes(MS5611_DEFAULT_ADDRESS, MS5611_RA_ADC, 3);
	//D2_ = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
}

// Calculate temperature and pressure calculations and perform compensation. More info about these calculations is available in the datasheet.
void BAR::CalculatePressureAndTemperature() 
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
    pres_ = (((D1_ * SENS) / pow(2, 21) - OFF) / pow(2, 15) / 100)*mbar_to_Pa;
    temp_ = temp_ / 100;
}

/** Perform pressure and temperature reading and calculation at once.
 *  Contains sleeps, better perform operations separately.
 */
void BAR::Update() 
{
    //refreshPressure();
    //usleep(10000); // Waiting for pressure data ready
    readPressure();

    //refreshTemperature();
    //usleep(10000); // Waiting for temperature data ready
    readTemperature();

    calculatePressureAndTemperature();
}