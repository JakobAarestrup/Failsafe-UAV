class IMU
{
public:
    IMU(); // Constructor
    ~IMU(); // Destructor
    void ConvertIMUData(int x, int y, int z, int sensor_handle); // Convert data from IMU to usable data
    float getRoll() const; // Returns roll
    float getPitch() const; // Returns pitch
    float getGyrX() const; // Returns gyroscope x-angle
    float getGyrY() const; // Returns gyroscope y-angle
    float getGyrZ()const; // Returns gyroscope z-angle
    int getMagX() const; // Returns magnometer x-angle
    int getMagY()const; // Returns magnometer y-angle
    int getMagZ()const; // Returns magnometer z-angle
private:
    int sensor_handle_; // Handle variable for ConvertIMUData for choosing sensor
    float gyr_Xangle_ = 0.0; // Gyroscope X-angle
	float gyr_Yangle_ = 0.0; // Gyroscope Y-angle
	float gyr_Zangle_ = 0.0; // Gyroscope Z-angle
    float rate_gyr_y_ = 0.0;   // Gyroscope X-angle in deg/s
	float rate_gyr_x_ = 0.0;   // Gyroscope Y-angle in deg/s
	float rate_gyr_z_ = 0.0;   // Gyroscope Z-angle in deg/s
    float roll_; // Roll in deg
    float pitch_; // Pitch in deg
    int mag_X_; // Magnetometer X-axis
    int mag_Y_; // Magnetometer Y-axis
    int mag_Z_; // Magnetometer Z-axis
};