#define LSM6DSOX_ADDR1 0x6A ///< LSM6DSOX default device id from WHOAMI
#define LSM6DSOX_ADDR2 0x6B ///< LSM6DSOX default device id from WHOAMI

#define LSM6DSOX_FUNC_CFG_ACCESS 0x1 ///< Enable embedded functions register
#define LSM6DSOX_PIN_CTRL 0x2        ///< Pin control register

#define LSM6DSOX_S4S_TPH_L 0x04        ///< Unused
#define LSM6DSOX_S4S_TPH_h 0x05        ///< Unused
#define LSM6DSOX_RR 0x06               ///< Unused
#define LSM6DSOX_FIFO_CTRL1 0x07       ///< Unused
#define LSM6DSOX_FIFO_CTRL2 0x08       ///< Unused
#define LSM6DSOX_FIFO_CTRL3 0x09       ///< Unused
#define LSM6DSOX_FIFO_CTRL4 0x0A       ///< Unused
#define LSM6DSOX_COUNTER_BDR_REG1 0x0B ///< Unused
#define LSM6DSOX_COUNTER_BDR_REG2 0x0C ///< Unused

#define LSM6DSOX_INT1_CTRL 0x0D ///< Interrupt enable for data ready
#define LSM6DSOX_INT2_CTRL 0x0E ///< Unused
#define LSM6DSOX_CTRL1_XL 0x10  ///< Main accelerometer config register. Should be written 0b10100000 to for: z,y,x axis enabled , 6.66kHz data rate, 2G full scale, no LP filter.
#define LSM6DSOX_CTRL2_G 0x11   ///< Main gyro config register
#define LSM6DSOX_CTRL3_C 0x12   ///< Main configuration register. Should be written 0b01000000 to for: BDU Enable (check for MSB and LSB). All other bits in default-mode.
#define LSM6DSOX_CTRL4_C 0x13   ///< Unused
#define LSM6DSOX_CTRL5_C 0x14   ///< I2C Master config; access must be enabled with bit SHUB_REG_ACCESS. Is set to '1' in FUNC_CFG_ACCESS (01h).
#define LSM6DSOX_CTRL6_C 0x15   ///< Unused
#define LSM6DSOX_CTRL7_G 0x16   ///< Unused
#define LSM6DSOX_CTRL8_XL 0x17  ///< Unused
#define LSM6DSOX_CTRL9_XL 0x18  ///< Includes i3c disable bit
#define LSM6DSOX_CTRL10_C 0x19  ///< Unused

#define LSM6DSOX_ALL_INT_SRC 0x1A ///< Unused
#define LSM6DSOX_WAKE_UP_SRC 0x1B ///< Unused
#define LSM6DSOX_TAP_SRC 0x1C     ///< Unused
#define LSM6DSOX_D6D_SRC 0x1D     ///< Unused
#define LSM6DSOX_STATUS_REG 0x1E  ///< Unused

// Temperature output registers
#define LSM6DSOX_OUT_TEMP_L 0x20 ///< Unused
#define LSM6DSOX_OUT_TEMP_H 0x21 ///< Unused

// Gyro output registers
#define LSM6DSOX_OUT_X_L_G 0x22 // First half of x-coordinates from Gyro
#define LSM6DSOX_OUT_X_H_G 0x23 // Second half of x-coordinates from Gyro
#define LSM6DSOX_OUT_Y_L_G 0x24 // First half of y-coordinates from Gyro
#define LSM6DSOX_OUT_Y_H_G 0x25 // Second half of y-coordinates from Gyro
#define LSM6DSOX_OUT_Z_L_G 0x26 // First half of z-coordinates from Gyro
#define LSM6DSOX_OUT_Z_H_G 0x27 // Second half of z-coordinates from Gyro

// Accelerometer output registers
#define LSM6DSOX_OUT_X_L_A 0x28 // First half of x-coordinates from Accelerometer
#define LSM6DSOX_OUT_X_H_A 0x29 // Second half of x-coordinates from Accelerometer
#define LSM6DSOX_OUT_Y_L_A 0x2A // First half of y-coordinates from Accelerometer
#define LSM6DSOX_OUT_Y_H_A 0x2B // Second half of y-coordinates from Accelerometer
#define LSM6DSOX_OUT_Z_L_A 0x2C // First half of z-coordinates from Accelerometer
#define LSM6DSOX_OUT_Z_H_A 0x2D // Second half of z-coordinates from Accelerometer

#define lSM6DSOX_EMB_FUNC_STATUS_MAINPAGE 0x35 ///< Unused
#define lSM6DSOX_FSM_STATUS_A_MAINPAGE 0x36    ///< Unused
#define lSM6DSOX_FSM_STATUS_B_MAINPAGE 0x37    ///< Unused
#define lSM6DSOX_MLC_STATUS_MAINPAGE 0x38      ///< Unused
#define lSM6DSOX_STATUS_MASTER_MAINPAGE 0x39   ///< Unused

#define lSM6DSOX_FIFO_STATUS1 0x3A ///< Unused
#define lSM6DSOX_FIFO_STATUS2 0x3B ///< Unused

#define lSM6DSOX_TIMESTAMP0 0x40 ///< Unused
#define lSM6DSOX_TIMESTAMP1 0x41 ///< Unused
#define lSM6DSOX_TIMESTAMP2 0x42 ///< Unused
#define lSM6DSOX_TIMESTAMP3 0x43 ///< Unused

#define lSM6DSOX_UI_STATUS_REG_OIS 0x49 ///< Unused
#define lSM6DSOX_UI_OUTX_L_G_OIS 0x4A   ///< Unused
#define lSM6DSOX_UI_OUTX_H_G_OIS 0x4B   ///< Unused
#define lSM6DSOX_UI_OUTY_L_G_OIS 0x4C   ///< Unused
#define lSM6DSOX_UI_OUTY_H_G_OIS 0x4D   ///< Unused
#define lSM6DSOX_UI_OUTZ_L_G_OIS 0x4E   ///< Unused
#define lSM6DSOX_UI_OUTZ_H_G_OIS 0x4F   ///< Unused
#define lSM6DSOX_UI_OUTX_L_A_OIS 0x50   ///< Unused
#define lSM6DSOX_UI_OUTX_H_A_OIS 0x51   ///< Unused
#define lSM6DSOX_UI_OUTY_L_A_OIS 0x52   ///< Unused
#define lSM6DSOX_UI_OUTY_H_A_OIS 0x53   ///< Unused
#define lSM6DSOX_UI_OUTZ_L_A_OIS 0x54   ///< Unused
#define lSM6DSOX_UI_OUTZ_H_A_OIS 0x55   ///< Unused

#define lSM6DSOX_TAP_CFG0 0x56     ///< Unused
#define lSM6DSOX_TAP_CFG1 0x57     ///< Unused
#define lSM6DSOX_TAP_CFG2 0x58     ///< Unused
#define lSM6DSOX_TAP_THIS_6D 0x59  ///< Unused
#define lSM6DSOX_INT_DUR2 0x5A     ///< Unused
#define lSM6DSOX_WAKE_UP_THIS 0x5B ///< Unused

#define lSM6DSOX_WAKE_UP_DUR 0x5C ///< W/ Used for free fall duration setting
#define lSM6DSOX_FREE_FALL 0x5D   ///< R/W Used for free fall detection

#define lSM6DSOX_MD1_CFG 0x5E            ///< Unused
#define lSM6DSOX_MD2_CFG 0x5F            ///< Unused
#define lSM6DSOX_S4S_ST_CMD_CODE 0x60    ///< Unused
#define lSM6DSOX_S4S_DT_REG 0x61         ///< Unused
#define lSM6DSOX_I3C_BUS_AVB 0x62        ///< Unused
#define lSM6DSOX_INTERNAL_FREQ_FINE 0x63 ///< Unused

#define lSM6DSOX_UI_INT_OIS 0x6F   ///< Unused
#define lSM6DSOX_UI_CTRL1_OIS 0x70 ///< Unused
#define lSM6DSOX_UI_CTRL2_OIS 0x71 ///< Unused
#define lSM6DSOX_UI_CTRL3_OIS 0x72 ///< Unused
#define lSM6DSOX_X_OFS_USR 0x73    ///< Unused
#define lSM6DSOX_Y_OFS_USR 0x74    ///< Unused
#define lSM6DSOX_Z_OFS_USR 0x75    ///< Unused

#define lSM6DSOX_FIFO_DATA_OUT_TAG 0x78 ///< Unused
#define lSM6DSOX_FIFO_DATA_OUT_X_L 0x79 ///< Unused
#define lSM6DSOX_FIFO_DATA_OUT_X_H 0x7A ///< Unused
#define lSM6DSOX_FIFO_DATA_OUT_Y_L 0x7B ///< Unused
#define lSM6DSOX_FIFO_DATA_OUT_Y_H 0x7C ///< Unused
#define lSM6DSOX_FIFO_DATA_OUT_Z_L 0x7D ///< Unused
#define lSM6DSOX_FIFO_DATA_OUT_Z_H 0x7E ///< Unused