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
    double magYaw_ = 0.0;
    double rate_gyr_y_ = 0.0;   // Gyroscope X-angle in deg/s
	double rate_gyr_x_ = 0.0;   // Gyroscope Y-angle in deg/s
	double rate_gyr_z_ = 0.0;   // Gyroscope Z-angle in deg/s
    double gyroXangle_ = 0.0; // Gy
	double gyroYangle_ = 0.0; // Gy
	double gyroZangle_ = 0.0;
    double XL_Roll_ = 0.0; // Accelerometer X-axis
    double XL_Pitch_ = 0.0; // Accelerometer Y-axis
};