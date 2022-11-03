class IMU
{
public:
    IMU(); // Constructor
    ~IMU(); // Destructor
    void ConvertGyroData(int gY, int gX, int gZ);
    void ConvertMagData(int mY, int mX, int mZ);
    //float getACCData(); // Returns ACC data
    float getMagData();
    float getGyroData(); // Returns Gyro data
/* 	float gyr_Zangle_ = 0.0; // Gyroscope Z-angle
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
    int mag_Z_; // Magnetometer Z-axis */
    float acc_X_; // Accelerometer X-axis
    float acc_Y_; // Accelerometer Y-axis
    float gyroData; // Gyroscope data
    float magData; // Magnetometer data
};