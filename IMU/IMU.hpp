class IMU
{
public:
    IMU(); // Constructor
    ~IMU(); // Destructor
<<<<<<< HEAD
    int ConvertACCData(int aY, int aX, int aZ);
    void ConvertGyroData(int gY, int gX, int gZ);
    void ConvertMagData(int mY, int mX, int mZ);
=======
    void ConvertIMUData(int x, int y, int z, int sensor_handle); // Convert data from IMU to usable data
    void FilterIMUData(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float, float mz); // Put IMU data through complimentary filter
>>>>>>> 888e9f7d30842553e7a748e7046f0153b545a794
    float getACCData(); // Returns ACC data
    float getGyroData(); // Returns Gyro data
    float getMagData(); // Returns Mag data
private:
    /*
    int sensor_handle_; // Handle variable for ConvertIMUData for choosing sensor
    float gyr_Xangle_ = 0.0; // Gyroscope X-angle
	float gyr_Yangle_ = 0.0; // Gyroscope Y-angle
	float gyr_Zangle_ = 0.0; // Gyroscope Z-angle
    float rate_gyr_y_ = 0.0;   // Gyroscope X-angle in deg/s
	float rate_gyr_x_ = 0.0;   // Gyroscope Y-angle in deg/s
	float rate_gyr_z_ = 0.0;   // Gyroscope Z-angle in deg/s
    int acc_X_; // Accelerometer X-axis
    int acc_Y_; // Accelerometer Y-axis
    int acc_Z_; // Accelerometer Z-axis
    int gyro_Y_; // Gyroscope Y-axis
    int gyro_Z_; // Gyroscope Z-axis
    int mag_X_; // Magnetometer X-axis
    int mag_Y_; // Magnetometer Y-axis
    int mag_Z_; // Magnetometer Z-axis
    */
    float acc_X_; // Accelerometer X-axis
    float acc_Y_; // Accelerometer Y-axis
    float gyroData; // Gyroscope data
    float magData; // Magnetometer data
};