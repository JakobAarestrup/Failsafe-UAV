class IMU
{
public:
    IMU(); // Constructor
    ~IMU(); // Destructor
    void update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    void ConvertIMUData(int x, int y, int z, int sensor_handle); // Convert data from IMU to usable data
    float getRoll() const; // Returns roll
    float getPitch() const; // Returns pitch
    float getYaw() const; // Returns yaw
    void computeAngles()
    float invSqrt(float x)
    void updateIMU(float gx, float gy, float gz, float ax, float ay, float az)
private:
    int sensor_handle_; // Handle variable for ConvertIMUData for choosing sensor
    float gyr_Xangle_ = 0.0; // Gyroscope X-angle
	float gyr_Yangle_ = 0.0; // Gyroscope Y-angle
	float gyr_Zangle_ = 0.0; // Gyroscope Z-angle
    float rate_gyr_y_ = 0.0;   // Gyroscope X-angle in deg/s
	float rate_gyr_x_ = 0.0;   // Gyroscope Y-angle in deg/s
	float rate_gyr_z_ = 0.0;   // Gyroscope Z-angle in deg/s
    float roll_; // Roll in deg
    float pitch_; // Pitch in deg
    float yaw_; // Yaw in deg
    int mag_X_; // Magnetometer X-axis
    int mag_Y_; // Magnetometer Y-axis
    int mag_Z_; // Magnetometer Z-axis
    float twoKp;        // 2 * proportional gain (Kp)
    float twoKi;        // 2 * integral gain (Ki)
    float q0, q1, q2, q3;   // quaternion of sensor frame relative to auxiliary frame
    float integralFBx, integralFBy, integralFBz;  // integral error terms scaled by Ki
    float invSampleFreq;
    static float invSqrt(float x);
    char anglesComputed;
};