class IMU
{
public:
    IMU(); // Constructor
    ~IMU(); // Destructor
    void ConvertACCData(int aX, int aY, int aZ);
    void ConvertGyroData(int gX, int gY, int gZ);
    void ConvertMagData(int mX, int mY);
    //float getACCData(); // Returns ACC data
    float getMagData();
    float getGyroData(); // Returns Gyro data
/* 	float gyr_Zangle_ = 0.0; // Gyroscope Z-angle
    
    int acc_X_; // Accelerometer X-axis
    int acc_Y_; // Accelerometer Y-axis
    int acc_Z_; // Accelerometer Z-axis
    int gyro_Y_; // Gyroscope Y-axis
    int gyro_Z_; // Gyroscope Z-axis
    */
    float magYaw_ = 0.0;
    float rate_gyr_y_ = 0.0;   // Gyroscope X-angle in deg/s
	float rate_gyr_x_ = 0.0;   // Gyroscope Y-angle in deg/s
	float rate_gyr_z_ = 0.0;   // Gyroscope Z-angle in deg/s
    float gyroXangle_ = 0.0; // Gy
	float gyroYangle_ = 0.0; // Gy
	float gyroZangle_ = 0.0;
    float acc_X_; // Accelerometer X-axis
    float acc_Y_; // Accelerometer Y-axis
};