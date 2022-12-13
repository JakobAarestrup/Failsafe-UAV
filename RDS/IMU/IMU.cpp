#include "IMU.hpp"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "I2C.hpp"
#include "LIS3MDL.hpp"
#include "LSM6DSOX.hpp"
#include <wiringPi.h>

/**
 * @brief Math constants used for calculations
 *
 */
/// Pi constant
#define PI 3.14159265358979323846
/// Radians to degrees constant
#define RAD_TO_DEG 57.29578
/// [s/loop] loop period. 20ms
#define DT 0.005
/// Complementary filter constant
#define AA 0.97
/// Accelerometer sensitivity constant
#define XL_Sensitivity 0.061
/// Gyroscope sensitivity constant
#define G_Sensitivity 70
/// Magnetometer sensitivity constant
#define MG_Sensitivity 6842
/// Gyroscope drift constant
#define gyroDrift 0.07

/**
 * @brief I2C class member
 *
 */
I2C I1;

/**
 * @brief Construct a new IMU::IMU object
 *
 */
IMU::IMU()
{
}

/**
 * @brief Destroy the IMU::IMU object
 *
 */
IMU::~IMU()
{
}

/**
 * @brief Function calibrates gyroscope from starting position
 *
 * @param IMU handler for choosing IMU sensor.
 */
void IMU::calibrateGyro(int IMU)
{
    printf("Calibrating gyro...\n");
    float gx_TOTAL = 0, gy_TOTAL = 0, gz_TOTAL = 0;
    int i = 0;
    int N = 500; // amount of samples

    for (i = 0; i < N; i++)
    {

        if (IMU == 1)
        {
            gx_TOTAL += I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_G, 1, 1);
            gy_TOTAL += I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_G, 1, 1);
            gz_TOTAL += I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_G, 1, 1);
        }
        else if (IMU == 2)
        {
            gx_TOTAL += I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G, 1, 1);
            gy_TOTAL += I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G, 1, 1);
            gz_TOTAL += I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G, 1, 1);
        }
        else
        {
            printf("IMU value is not supported...\n");
            return;
        }
    }
    /**
     * @brief Convert 16 bit register from mdeg/s/LSB to deg/s
     *
     */
    gx_drift_ = ((gx_TOTAL * G_Sensitivity) / 1000) / N;
    gy_drift_ = ((gy_TOTAL * G_Sensitivity) / 1000) / N;
    gz_drift_ = ((gz_TOTAL * G_Sensitivity) / 1000) / N;
}

/**
 * @brief Function reads data of the three sensors and subtracting offsets.
 *
 * @param IMU handler for choosing IMU sensor.
 */

void IMU::readIMU(int IMU)
{
    readACC(IMU);
    readGYRO(IMU);
    readMAG(IMU);
}

/**
 * @brief Function reads values from the accelerometer
 *
 * @param IMU handler for choosing IMU sensor.
 */
void IMU::readACC(int IMU)
{
    float ax, ay, az;

    /**
     * @brief handler for IMU sensor
     *
     */
    if (IMU == 1)
    {
        ax = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_A, 1, 1);
        ay = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_A, 1, 1);
        az = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_A, 1, 1);
    }
    else if (IMU == 2)
    {
        ax = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_A, 1, 1);
        ay = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_A, 1, 1);
        az = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_A, 1, 1);
    }
    else
    {
        printf("IMU value is not supported...\n");
        return;
    }

    /**
     * @brief Convert 16 bit register from mg/LSB to g
     *
     */
    ax = (ax * XL_Sensitivity) / 1000;
    ay = (ay * XL_Sensitivity) / 1000;
    az = (az * XL_Sensitivity) / 1000;

    /**
     * @brief Calibration could be implemented here
     *
     */
    accCalibX_ = ax;
    accCalibY_ = ay;
    accCalibZ_ = az;
}

/**
 * @brief Function reads values from the gyroscope
 *
 * @param IMU handler for choosing IMU sensor.
 */
void IMU::readGYRO(int IMU)
{
    float gx, gy, gz;

    /// @brief handler for IMU sensor
    if (IMU == 1)
    {
        gx = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_X_L_G, 1, 1);
        gy = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Y_L_G, 1, 1);
        gz = I1.readI2C(LSM6DSOX_ADDR1, LSM6DSOX_OUT_Z_L_G, 1, 1);
    }
    else if (IMU == 2)
    {
        gx = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_X_L_G, 1, 1);
        gy = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Y_L_G, 1, 1);
        gz = I1.readI2C(LSM6DSOX_ADDR2, LSM6DSOX_OUT_Z_L_G, 1, 1);
    }
    else
    {
        printf("IMU value is not supported...\n");
        return;
    }

    ///@brief Convert 16 bit register from mdeg/s/LSB to deg/s
    gx = (gx * G_Sensitivity) / 1000;
    gy = (gy * G_Sensitivity) / 1000;
    gz = (gz * G_Sensitivity) / 1000;

    /* Calibration offset */

    gyroCalibX_ = gx - gx_drift_; // Gyroscope X-angle in deg/s
    gyroCalibY_ = gy - gy_drift_; // Gyroscope Y-angle in deg/s
    gyroCalibZ_ = gz - gz_drift_; // Gyroscope Z-angle in deg/s
}

/**
 * @brief Function reads values from the magnetometer
 *
 * @param IMU handler for choosing IMU sensor.
 */
void IMU::readMAG(int IMU)
{
    float mx, my, mz;
    float bx, by, bz;

    /// @brief handler for IMU sensor
    if (IMU == 1)
    {
        mx = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_X_L, 1, 1);
        my = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Y_L, 1, 1);
        mz = I1.readI2C(LIS3MDL_ADDR1, LIS3MDL_OUT_Z_L, 1, 1);
    }
    else if (IMU == 2)
    {
        mx = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_X_L, 1, 1);
        my = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_Y_L, 1, 1);
        mz = I1.readI2C(LIS3MDL_ADDR2, LIS3MDL_OUT_Z_L, 1, 1);
    }
    else
    {
        printf("IMU value is not supported...\n");
        return;
    }

    /// @brief Convert 16 bit register from LSB/guass to uT (micro Tesla)
    mx = (mx / MG_Sensitivity) * 100;
    my = (my / MG_Sensitivity) * 100;
    mz = (mz / MG_Sensitivity) * 100;

    /// @brief hardcoded offsets. b = Hard-Iron distortion and A = Soft-Iron distortion.
    if (IMU == 1)
    {
        bx = 30.046034;
        by = -8.930363;
        bz = 16.782248;

        float A[3][3] = {{0.923293, 0.014763, -0.000824},
                         {0.014763, 0.911495, -0.041387},
                         {-0.000824, -0.041387, 0.996074}};
        // formel magdataCalibrated = A(magdata-b)
        mx = mx - bx;
        my = my - by;
        mz = mz - bz;

        magCalibX_ = A[0][0] * mx + A[0][1] * my + A[0][2] * mz; // A[0,:]*(magdata-b)
        magCalibY_ = A[1][0] * mx + A[1][1] * my + A[1][2] * mz; // A[1,:]*(magdata-b)
    }
    else if (IMU == 2) // TODO ny calibreringsdata
    {
        bx = 7.977849;
        by = 3.137438;
        bz = -5.371644;

        float A[3][3] = {{1.002979, 0.039343, -0.014713},
                         {0.039343, 1.019943, -0.006826},
                         {-0.014713, -0.006826, 1.014517}};
        // formel magdataCalibrated = A(magdata-b)
        mx = mx - bx;
        my = my - by;
        mz = mz - bz;

        magCalibX_ = A[0][0] * mx + A[0][1] * my + A[0][2] * mz; // A[0,:]*(magdata-b)
        magCalibY_ = A[1][0] * mx + A[1][1] * my + A[1][2] * mz; // A[1,:]*(magdata-b)
    }
}

/**
 * @brief Function converts accelerometer data to roll and pitch
 *
 */
void IMU::convertAccData()
{
    /*  gyroXangle_ = PI*(rate_gyr_x_ / (DT*1000));
   gyroYangle_ = PI*(rate_gyr_y_ / (DT*1000));
   gyroZangle_ = PI*(rate_gyr_z_ / (DT*1000));  */
    xlRoll_ = atan2(accCalibY_, accCalibZ_) * 180 / PI;
    xlPitch_ = atan2(-accCalibX_, sqrt(accCalibY_ * accCalibY_ + accCalibZ_ * accCalibZ_)) * -180 / PI;
}

/**
 * @brief Function converts magnetometer data to yaw
 *
 */
void IMU::convertMagData()
{
    magYaw_ = (atan2(magCalibY_, magCalibX_) * 180 / PI); // minus magnetic_decline
    if (magYaw_ < 0)                                      // correct yaw if under 0 degrees
    {
        magYaw_ += 360;
        // printf("magYaw: %f\n\n", magYaw_);
    }
    // printf("YAW: %f\n", magYaw_);
}

/**
 * @brief Complimentary filter for roll, pitch and yaw
 *
 */
void IMU::complementaryFilter()
{
    /* printf("xlRoll_ %f xlPitch_%f\n", xlRoll_, xlPitch_);
    printf("YAW: %f\n", magYaw_); */
    /*Complementary Filter*/
    {
        std::scoped_lock<std::mutex> lock(m_);
        compRoll_ = AA * (compRoll_ + gyroCalibY_ * DT) + (1 - AA) * xlRoll_;    // 97% Gyro 3% Accelerometer
        compPitch_ = AA * (compPitch_ + gyroCalibX_ * DT) + (1 - AA) * xlPitch_; // 97% Gyro 3% Accelerometer
        compYaw_ = 0.5 * (compYaw_ + gyroCalibZ_ * DT) + (1 - 0.5) * magYaw_;    // 50% Gyro 50% Magnometer
    }
    // printf("Roll_filtered: %f, Pitch filtered: %f, GyroZangle: %f\n", compRoll_, compPitch_, compYaw_);
}

/**
 * @brief Function that checks for free fall via IMU
 *
 * @param IMU handler for choosing IMU sensor.
 * @return int freeFall
 */
/*int IMU::freeFall(int IMU)
{
    int freeFall = 0;
    I1.writeByte(LSM6DSOX_ADDR1, lSM6DSOX_WAKE_UP_DUR, 0b00000000); // 0x00 - Set event duration (FF_DUR5 bit)
    I1.writeByte(LSM6DSOX_ADDR1, lSM6DSOX_FREE_FALL, 0b00110111);   // 0x33 - Set FF threshold (FF_THS[2:0] = 500mg)
    I1.writeByte(LSM6DSOX_ADDR2, lSM6DSOX_WAKE_UP_DUR, 0b00000000); // 0x00 - Set event duration (FF_DUR5 bit)
    I1.writeByte(LSM6DSOX_ADDR2, lSM6DSOX_FREE_FALL, 0b00110111);   // 0x33 - Set FF threshold (FF_THS[2:0] = 500mg)

     if (IMU == 1)
     {
         fall_ = digitalRead(28);

         if (fall_ == 1)
         {
             I1.WriteI2C(LSM6DSOX_ADDR1, lSM6DSOX_WAKE_UP_DUR, 1, 0b00000000); // 0x00 - Set event duration (FF_DUR5 bit)
             I1.WriteI2C(LSM6DSOX_ADDR1, lSM6DSOX_FREE_FALL, 1, 0b00110000);   // 0x33 - Set FF threshold (FF_THS[2:0] = 156mg)
             freeFall = digitalRead(28);                                       // replace 1 with output of interrupt status on GPIO pin 28 for IMU1
         }
     }
     else if (IMU == 2)
     {
         fall_ = digitalRead(29); // output of interrupt status on GPIO pin 21 for IMU2
         if (fall_ == 1)
         {
             I1.WriteI2C(LSM6DSOX_ADDR2, lSM6DSOX_WAKE_UP_DUR, 1, 0b00000000); // 0x00 - Set event duration (FF_DUR5 bit)
             I1.WriteI2C(LSM6DSOX_ADDR2, lSM6DSOX_FREE_FALL, 1, 0b00110000);   // 0x33 - Set FF threshold (FF_THS[2:0] = 156mg)
             freeFall = digitalRead(29);                                       // output of interrupt status on GPIO pin 29 for IMU2
         }
     }
     return freeFall;
}
*/

/**
 * @brief Function places roll, pitch and yaw in struct
 *
 * @return Orientation
 */
Orientation IMU::getOrientation()
{

    float roll, pitch, yaw;

    {
        std::scoped_lock<std::mutex> lock(m_);
        roll = getRoll();
        pitch = getPitch();
        yaw = getYaw();
    }

    return {roll, pitch, yaw};
}

/**
 * @brief Function returns compensated roll
 *
 * @return float compRoll_
 */
float IMU::getRoll() const
{
    return compRoll_;
}

/**
 * @brief Function returns compensated pitch
 *
 * @return float compPitch_
 */
float IMU::getPitch() const
{
    return compPitch_;
}

/**
 * @brief Function returns compensated yaw
 *
 * @return float compYaw_
 */
float IMU::getYaw() const
{
    return compYaw_;
}
