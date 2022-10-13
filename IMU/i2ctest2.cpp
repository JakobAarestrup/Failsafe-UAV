#include <iostream>
#include <thread>
#include <chrono>
#include <wiringPiI2C.h>

#define DEVICE_ID 0x6a

#define REG_POWER_CTL   0x2D
#define REG_DATA_X_LOW  0x32
#define REG_DATA_X_HIGH 0x33
#define REG_DATA_Y_LOW  0x34
#define REG_DATA_Y_HIGH 0x35
#define REG_DATA_Z_LOW  0x36
#define REG_DATA_Z_HIGH 0x37

#define LSM6DSOX_FUNC_CFG_ACCESS 0x1 ///< Enable embedded functions register
#define LSM6DSOX_PIN_CTRL 0x2        ///< Pin control register

#define LSM6DSOX_INT1_CTRL 0x0D ///< Interrupt enable for data ready
#define LSM6DSOX_CTRL1_XL 0x10  ///< Main accelerometer config register
#define LSM6DSOX_CTRL2_G 0x11   ///< Main gyro config register
#define LSM6DSOX_CTRL3_C 0x12   ///< Main configuration register
#define LSM6DSOX_CTRL9_XL 0x18  ///< Includes i3c disable bit

void enableI2CMasterPullups(bool enable_pullups);
void disableSPIMasterPullups(bool disable_pullups);
int fd;

int main (int argc, char **argv)
{
    // Setup I2C communication
    int fd = wiringPiI2CSetup(DEVICE_ID);
    if (fd == -1) {
        std::cout << "Failed to init I2C communication.\n";
        return -1;
    }
    std::cout << "I2C communication successfully setup.\n";

    int wiringPiI2CRead(fd);

    return 0;
}