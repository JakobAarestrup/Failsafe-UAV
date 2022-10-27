class BAR
{
public:
    BAR(); // Constructor
    ~BAR();// Destructor
    void ConvertBARData(int x); // Converts the bar data into height
    void Calibrate_BAR(int x); // Calibrates the barometer data
    int  getHeight() const; // Returns the height above ground level
private:
    int initial_AMSL_; // Initial height above mean sea level when drone is on the ground
    int initial_pressure_;  // Initial pressure when drone is on the ground
    int height_AMSL_; // Height above mean sea level
    int height_AGL_; // Height above ground level
    uint32_t pressure_; // Pressure from barometer