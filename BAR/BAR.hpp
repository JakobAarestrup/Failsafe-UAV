 #include <stdint.h>

class BAR
{
public:
    BAR(); // Constructor
    ~BAR();// Destructor
    void ConvertBARData(int x); // Converts the bar data into height
    void Calibrate_BAR(); // Calibrates the barometer data
    int  getHeight() const; // Returns the height above ground level
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
private:
    int initial_AMSL_; // Initial height above mean sea level when drone is on the ground
    int initial_pressure_;  // Initial pressure when drone is on the ground
    int height_AMSL_; // Height above mean sea level
    int height_AGL_; // Height above ground level
    int pressure_; // Pressure from barometer
    int BAR_I2C_;
    uint16_t C1_, C2_, C3_, C4_, C5_, C6_; // Calibration data
    uint32_t D1_, D2_; // Raw measurement data
    float TEMP_; // Calculated temperature
	float PRES_; // Calculated pressure
};