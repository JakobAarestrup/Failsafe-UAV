#pragma once
#include <mutex>

struct Orientation
{
    float roll, pitch, yaw;
};

class IMU
{
public:
    IMU();
    ~IMU();
    void calibrateGyro(int IMU);
    void readIMU(int IMU);
    void readACC(int IMU);
    void readGYRO(int IMU);
    void readMAG(int IMU);
    void convertAccData();
    void convertMagData();
    void complementaryFilter();

    Orientation getOrientation();
    float getRoll() const;
    float getPitch() const;
    float getYaw() const;

private:
    /// @brief Accelerometer calibrated x-axis in deg/s
    float accCalibX_ = 0.0;
    /// @brief Accelerometer calibrated y-axis in deg/s
    float accCalibY_ = 0.0;
    /// @brief Accelerometer calibrated z-axis in deg/s
    float accCalibZ_ = 0.0;

    /// @brief Gyroscope calibrated x-axis in deg/s
    float gyroCalibX_ = 0.0;
    /// @brief Gyroscope calibrated y-axis in deg/s
    float gyroCalibY_ = 0.0;
    /// @brief Gyroscope calibrated z-axis in deg/s
    float gyroCalibZ_ = 0.0;

    /// @brief Gyroscope drift in x-axis in
    float gx_drift_ = 0.0;
    /// @brief Gyroscope drift in y-axis in
    float gy_drift_ = 0.0;
    /// @brief Gyroscope drift in z-axis in
    float gz_drift_ = 0.0;

    /// @brief Magnetometer calibrated x-axis in deg/s
    float magCalibX_ = 0.0;
    /// @brief Magnetometer calibrated y-axis in deg/s
    float magCalibY_ = 0.0;

    /// @brief Accelerometer roll
    float xlRoll_ = 0.0;
    /// @brief Accelerometer pitch
    float xlPitch_ = 0.0;
    /// @brief Magnetometer yaw
    float magYaw_ = 0.0;

    /// @brief Compensated roll
    float compRoll_ = 0.0;
    /// @brief Compensated pitch
    float compPitch_ = 0.0;
    /// @brief Compensated yaw
    float compYaw_ = 0.0;

    /// @brief Mutex object m_
    std::mutex m_;

    // int fall_;
};