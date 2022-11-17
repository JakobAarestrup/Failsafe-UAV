class IMU
{
public:
    IMU(); // Constructor
    ~IMU(); // Destructor
    void calibrateGyro(int IMU);
    void readIMU(int IMU); 
    void readACC(int IMU);
    void readGYRO(int IMU);
    void readMAG(int IMU);
    void ConvertACCData();
    void ConvertGyroData();
    void ConvertMagData();   
private:
    double rate_gyr_y_ = 0.0;   // Gyroscope X-angle in deg/s
	double rate_gyr_x_ = 0.0;   // Gyroscope Y-angle in deg/s
	double rate_gyr_z_ = 0.0;   // Gyroscope Z-angle in deg/s
    double gyroXangle_ = 0.0; // Gy
	double gyroYangle_ = 0.0; // Gy
	double gyroZangle_ = 0.0;
    double XL_Roll_ = 0.0; // Accelerometer X-axis
    double XL_Pitch_ = 0.0; // Accelerometer Y-axis
    double magYaw_ = 0.0;
    float magCalibX_;
    float magCalibY_;
    //float magCalibZ_;
};