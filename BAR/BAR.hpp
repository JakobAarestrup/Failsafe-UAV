 #include <stdint.h>

class BAR
{
public:
    BAR(); // Constructor
    ~BAR();// Destructor
    void ConvertBARData(); // Converts the bar data into height
    void Calibrate_BAR(); // Calibrates the barometer data
    void initialize();
    bool testConnection();
    void refreshPressure();
    void readPressure();
    void refreshTemperature();
    void readTemperature();
    void calculatePressureAndTemperature();
    void update();
    float getTemperature();
    float getPressure();
    float  getHeight(); // Returns the height above ground level
private:
    float initial_AMSL_; // Initial height above mean sea level when drone is on the ground
    float initial_pressure_;  // Initial pressure when drone is on the ground
    float height_AMSL_; // Height above mean sea level
    float height_AGL_; // Height above ground level
    float pressure_; // Pressure from barometer
    int BAR_I2C_;
    uint16_t C1_, C2_, C3_, C4_, C5_, C6_; // Calibration data
    uint32_t D1_, D2_; // Raw measurement data
    float TEMP_; // Calculated temperature
	float PRES_; // Calculated pressure
    float deviation_ = 0; // Measured deviation
    int calibration_ = 0; // Calibration variable for getHeight()
};