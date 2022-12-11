#include <iostream>
#include <cmath>

// Struct for a quaternion
struct Quaternion
{
    double w;
    double x;
    double y;
    double z;
};

// Function to convert a quaternion to roll, pitch, and yaw
void quatToEuler(const Quaternion &q, double &roll, double &pitch, double &yaw)
{
    // Compute roll
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    roll = std::atan2(sinr_cosp, cosr_cosp);

    // Compute pitch
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        pitch = std::asin(sinp);

    // Compute yaw
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    yaw = std::atan2(siny_cosp, cosy_cosp);
}

int main()
{
    // Example quaternion
    Quaternion q = {0.5, 0.5, 0.5, 0.5};

    // Convert to Euler angles
    double roll, pitch, yaw;
    quatToEuler(q, roll, pitch, yaw);

    // Convert angles to degrees
    roll = roll * 180 / M_PI;
    pitch = pitch * 180 / M_PI;
    yaw = yaw * 180 / M_PI;

    // Print result
    std::cout << "Roll: " << roll << " degrees" << std::endl;
    std::cout << "Pitch: " << pitch << " degrees" << std::endl;
    std::cout << "Yaw: " << yaw << " degrees" << std::endl;

    return 0;
}