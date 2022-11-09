#define LIS3MDL_ADDR_1 0x1C ///< LSM6DSOX default device id from WHOAMI
#define LIS3MDL_ADDR_2 0x1E ///< LSM6DSOX default device id from WHOAMI

#define LIS3MDL_WHO_AM_I 0x0F // Unused
#define LIS3MDL_CTRL_REG1 0x20 // should be written 0b01011100 to for: OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
#define LIS3MDL_CTRL_REG2 0x21 // should be written 0b00000000 to for: FS = 00 (+/- 4 gauss full scale)
#define LIS3MDL_CTRL_REG3 0x22 // should be written 0b00000000 to for: MD = 00 (continuous-conversion mode)
#define LIS3MDL_CTRL_REG4 0x23 // should be written 0b00001100 to for: OMZ = 11 (ultra-high-performance mode for Z)
#define LIS3MDL_CTRL_REG5 0x24 // Unused

#define LIS3MDL_STATUS_REG 0x27 // Unused
#define LIS3MDL_OUT_X_L 0x28 // Unused
#define LIS3MDL_OUT_X_H 0x29 // Unused
#define LIS3MDL_OUT_Y_L 0x2A // Unused
#define LIS3MDL_OUT_Y_H 0x2B // Unused
#define LIS3MDL_OUT_Z_L 0x2C // Unused
#define LIS3MDL_OUT_Z_H 0x2D // Unused
#define LIS3MDL_TEMP_OUT_L 0x2E // Unused
#define LIS3MDL_TEMP_OUT_H 0x2F // Unused

#define LIS3MDL_INT_CFG 0x30 // Unused
#define LIS3MDL_INT_SRC 0x31 // Unused
#define LIS3MDL_INT_THIS_L 0x32 // Unused
#define LIS3MDL_INT_THIS_H 0x33 // Unused