class IMU
{
public:
    IMU();
    ~IMU();
    void ConvertIMUData(int x, int y, int z, int sensor_handle);
    double roll_;
    double pitch_;
private:
    int sensor_handle_;

    float gyr_Xangle_ = 0.0;
	float gyr_Yangle_ = 0.0;
	float gyr_Zangle_ = 0.0;
    float rate_gyr_y_ = 0.0;   // [deg/s]
	float rate_gyr_x_ = 0.0;   // [deg/s]
	float rate_gyr_z_ = 0.0;   // [deg/s]
};