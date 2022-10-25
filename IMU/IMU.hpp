class IMU
{
public:
    IMU();
    ~IMU();
    void ConvertIMUData(int x, int y, int z, int sensor_handle);
    float getRoll() const;
    float getPitch() const;
    float getGyrX() const;
    float getGyrY() const;
    float getGyrZ()const;
    int getMagX() const;
    int getMagY()const;
    int getMagZ()const;
private:
    int sensor_handle_;
    float gyr_Xangle_ = 0.0;
	float gyr_Yangle_ = 0.0;
	float gyr_Zangle_ = 0.0;
    float rate_gyr_y_ = 0.0;   // [deg/s]
	float rate_gyr_x_ = 0.0;   // [deg/s]
	float rate_gyr_z_ = 0.0;   // [deg/s]
    float roll_;
    float pitch_;
    int mag_X_;
    int mag_Y_;
    int mag_Z_;
};