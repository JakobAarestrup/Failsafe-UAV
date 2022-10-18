#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <cmath.h>


#define LSM6DSOX_CHIP_ID 0x6b ///< LSM6DSOX default device id from WHOAMI
#define LSM6DSOX_ACC 0x6b

#define LSM6DSOX_FUNC_CFG_ACCESS 0x1 ///< Enable embedded functions register
#define LSM6DSOX_PIN_CTRL 0x2        ///< Pin control register

#define LSM6DSOX_INT1_CTRL 0x0D ///< Interrupt enable for data ready
#define LSM6DSOX_CTRL1_XL 0x10  ///< Main accelerometer config register
#define LSM6DSOX_CTRL2_G 0x11   ///< Main gyro config register
#define LSM6DSOX_CTRL3_C 0x12   ///< Main configuration register
#define LSM6DSOX_CTRL9_XL 0x18  ///< Includes i3c disable bit

// Accelerometer output registers
#define lSM6DSOX_ACC_OUT_X_L_A 0x28 // X1
#define lSM6DSOX_ACC_OUT_X_L_H 0x29 // X2
#define lSM6DSOX_ACC_OUT_Y_L_A 0x2a // Y1
#define lSM6DSOX_ACC_OUT_Y_L_H 0x2b // Y2
#define lSM6DSOX_ACC_OUT_Z_L_A 0x2c // Z1
#define lSM6DSOX_ACC_OUT_Z_L_A 0x2d // Z2

#define LSM6DSOX_MASTER_CONFIG 0x14
///< I2C Master config; access must be enabled with  bit SHUB_REG_ACCESS
///< is set to '1' in FUNC_CFG_ACCESS (01h).

#define M_PI = 3.14159265358979323846
#define RAD_TO_DEG = 57.29578

void selectDevice(int file, int addr)
{
        char device[3] == "LSM";        

     if (ioctl(file, I2C_SLAVE, addr) < 0) {
                fprintf(stderr,
                        "Error: Could not select device  0x%02x: %s\n",
                        device, strerror(errno));
        }
}

void readACC(int  *a , int file_i2c)
{
    uint8_t block[6];
    selectDevice(file_i2c,LSM6DSOX_CHIP_ID);
    readBlock(0x80 |  lSM6DSOX_ACC_OUT_X_L_A, sizeof(block), block);
     
    // Combine readings for each axis.
    *a = (int16_t)(block[0] | block[1] << 8);
    *(a+1) = (int16_t)(block[2] | block[3] << 8);
    *(a+2) = (int16_t)(block[4] | block[5] << 8);
}

void writeAccReg(uint8_t reg, uint8_t value, int file)
{
    selectDevice(file,LSM6DSOX_ACC);
  int result = i2c_smbus_write_byte_data(file, reg, value);
    if (result == -1)
    {
        printf ("Failed to write byte to I2C Acc.");
        exit(1);
    }
}

int main()
{
int file_i2c;
int* acc_raw;
float AccXangle;
float AccYangle;
const int addr = LSM6DSOX_CHIP_ID;

// // Setup I2C communication
//     int fd = wiringPiI2CSetup(LSM6DSOX_CHIP_ID);
//     if (fd == -1) {
//         std::cout << "Failed to init I2C communication.\n";
//         return -1;
//     }
//     std::cout << "I2C communication successfully setup.\n";

char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}


// Enable accelerometer.
writeAccReg(LSM6DSOX_CTRL1_XL, 0b10100000, file_i2c); //  z,y,x axis enabled , 6.66kHz data rate, 2G full scale, no LP filter.
writeAccReg(LSM6DSOX_CTRL3_C, 0b01000000, file_i2c); // enable BDU (check for MSB and LSB). All other bits in default-mode.

while(1)
{
    readACC(acc_raw, file_i2c);

    //Convert Accelerometer values to degrees
    AccXangle = (float) (atan2(*(acc_raw+1),*(acc_raw+2))+M_PI)*RAD_TO_DEG;
    AccYangle = (float) (atan2(*(acc_raw+2),*acc_raw)+M_PI)*RAD_TO_DEG;


    //Change the rotation value of the accelerometer to -/+ 180
    if (AccXangle > 180)
        {      
            AccXangle -= (float)360.0;
        }

    if (AccYangle > 180)
        {
            AccYangle -= (float)360.0;
        }
    printf("X-angle: %f", AccXangle);
    printf("Y-angle: %f", AccYangle);
    usleep(1000000);

}
}