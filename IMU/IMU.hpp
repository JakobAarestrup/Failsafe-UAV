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
    
    float accCalibX_;
    float accCalibY_;
    float accCalibZ_;

    float gyroCalibX_; ///< Gyroscope calibrated x-axis in deg/s
    float gyroCalibY_; ///< Gyroscope calibrated y-axis in deg/s
    float gyroCalibZ_; ///< Gyroscope calibrated x-axis in deg/s

    float magCalibX_;
    float magCalibY_;
    //float magCalibZ_;

    float XL_Roll_ = 0.0; // Accelerometer X-axis
    float XL_Pitch_ = 0.0; // Accelerometer Y-axis
    float magYaw_;
    
};