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
    float accCalibX_;
    /// @brief Accelerometer calibrated y-axis in deg/s
    float accCalibY_;
    /// @brief Accelerometer calibrated z-axis in deg/s
    float accCalibZ_;

    /// @brief Gyroscope calibrated x-axis in deg/s
    float gyroCalibX_;
    /// @brief Gyroscope calibrated y-axis in deg/s
    float gyroCalibY_;
    /// @brief Gyroscope calibrated z-axis in deg/s
    float gyroCalibZ_;

    /// @brief Gyroscope drift in x-axis in
    float gx_drift_;
    /// @brief Gyroscope drift in y-axis in
    float gy_drift_;
    /// @brief Gyroscope drift in z-axis in
    float gz_drift_;

    /// @brief Magnetometer calibrated x-axis in deg/s
    float magCalibX_;
    /// @brief Magnetometer calibrated y-axis in deg/s
    float magCalibY_;

    /// @brief Accelerometer roll
    float xlRoll_ = 0.0;
    /// @brief Accelerometer pitch
    float xlPitch_ = 0.0;
    /// @brief Magnetometer yaw
    float magYaw_;

    /// @brief Compensated roll
    float compRoll_;
    /// @brief Compensated pitch
    float compPitch_;
    /// @brief Compensated yaw
    float compYaw_;

    /// @brief Mutex object m_
    std::mutex m_;

    // int fall_;
};