class IMU
{
public:
    IMU();
    ~IMU();
    void ConvertIMUData(int x, int y, int z, int sensor_handle);
    double getRoll();
    double getPitch();
    double getGyrX();
    double getGyrY();
    double getGyrZ();
    double getMagX();
    double getMagY();
    double getMagZ();
private:
    int sensor_handle_;
    float gyr_Xangle_ = 0.0;
	float gyr_Yangle_ = 0.0;
	float gyr_Zangle_ = 0.0;
    float rate_gyr_y_ = 0.0;   // [deg/s]
	float rate_gyr_x_ = 0.0;   // [deg/s]
	float rate_gyr_z_ = 0.0;   // [deg/s]
    double roll_;
    double pitch_;
    float mag_X_ = 0.0;
    float mag_Y_ = 0.0;
    float mag_Z_ = 0.0;
};