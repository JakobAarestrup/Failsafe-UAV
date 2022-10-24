#define LIS3MDL_ADDR_1 0x1C ///< LSM6DSOX default device id from WHOAMI
#define LIS3MDL_ADDR_2 0x1E ///< LSM6DSOX default device id from WHOAMI

#define LIS3MDL_WHO_AM_I 0x0F
#define LIS3MDL_CTRL_REG1 0x20
// should be written 0b01011100 to for: Temperature sensor disabled, high performance mode, 80Hz ODR, self test disabled.
#define LIS3MDL_CTRL_REG2 0x21
// should be written 0b01100000 to for: +-16 gauss full-scale selection.
#define LIS3MDL_CTRL_REG3 0x22
#define LIS3MDL_CTRL_REG4 0x23
#define LIS3MDL_CTRL_REG5 0x24

#define LIS3MDL_STATUS_REG 0x27
#define LIS3MDL_OUT_X_L 0x28
#define LIS3MDL_OUT_X_H 0x29
#define LIS3MDL_OUT_Y_L 0x2A
#define LIS3MDL_OUT_Y_H 0x2B
#define LIS3MDL_OUT_Z_L 0x2C
#define LIS3MDL_OUT_Z_H 0x2D
#define LIS3MDL_TEMP_OUT_L 0x2E
#define LIS3MDL_TEMP_OUT_H 0x2F

#define LIS3MDL_INT_CFG 0x30
#define LIS3MDL_INT_SRC 0x31
#define LIS3MDL_INT_THIS_L 0x32
#define LIS3MDL_INT_THIS_H 0x33