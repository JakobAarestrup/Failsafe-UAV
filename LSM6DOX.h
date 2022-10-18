#define LSM6DSOX_CHIP_ID 0x6B ///< LSM6DSOX default device id from WHOAMI

#define LSM6DSOX_FUNC_CFG_ACCESS 0x1 ///< Enable embedded functions register
#define LSM6DSOX_PIN_CTRL 0x2        ///< Pin control register

#define LSM6DSOX_INT1_CTRL 0x0D ///< Interrupt enable for data ready
#define LSM6DSOX_CTRL1_XL 0x10  ///< Main accelerometer config register
#define LSM6DSOX_CTRL2_G 0x11   ///< Main gyro config register
#define LSM6DSOX_CTRL3_C 0x12   ///< Main configuration register
#define LSM6DSOX_CTRL9_XL 0x18  ///< Includes i3c disable bit

#define LSM6DSOX_MASTER_CONFIG 0x14
///< I2C Master config; access must be enabled with  bit SHUB_REG_ACCESS
///< is set to '1' in FUNC_CFG_ACCESS (01h).

// Accelerometer output registers
#define lSM6DSOX_ACC_OUT_X_L_A 0x28 // X1
#define lSM6DSOX_ACC_OUT_X_L_H 0x29 // X2
#define lSM6DSOX_ACC_OUT_Y_L_A 0x2a // Y1
#define lSM6DSOX_ACC_OUT_Y_L_H 0x2b // Y2
#define lSM6DSOX_ACC_OUT_Z_L_A 0x2c // Z1
#define lSM6DSOX_ACC_OUT_Z_L_A 0x2d // Z2