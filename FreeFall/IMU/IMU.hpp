#pragma once
class IMU
{
public:
    IMU();  // Constructor
    ~IMU(); // Destructor
    void calibrateGyro(int IMU);
    void readIMU(int IMU);
    void readACC(int IMU);
    void readGYRO(int IMU);
    void readMAG(int IMU);
    void ConvertACCData();
    void ConvertMagData();
    void ComplementaryFilter();
    int freeFall(int IMU);

    float getRoll() const;
    float getPitch() const;
    float getYaw() const;

private:
    float accCalibX_;
    float accCalibY_;
    float accCalibZ_;

    float gyroCalibX_; ///< Gyroscope calibrated x-axis in deg/s
    float gyroCalibY_; ///< Gyroscope calibrated y-axis in deg/s
    float gyroCalibZ_; ///< Gyroscope calibrated x-axis in deg/s

    float gx_drift;
    float gy_drift;
    float gz_drift;

    float magCalibX_;
    float magCalibY_;

    float XL_Roll_ = 0.0;  // Accelerometer X-axis
    float XL_Pitch_ = 0.0; // Accelerometer Y-axis
    float MAG_Yaw_;

    float CompRoll_;
    float CompPitch_;
    float CompYaw_;

    int fall_;
};