#ifndef _I2Cdev_H_
#define _I2Cdev_H_

#define RASPBERRY_PI_I2C "/dev/i2c-1"
#define BANANA_PI_I2C "/dev/i2c-2"

#define I2CDEV RASPBERRY_PI_I2C

#ifndef TRUE
#define TRUE (1 == 1)
#define FALSE (0 == 1)
#endif

#include <stdint.h>

class I2Cdev
{
public:
    I2Cdev();

    static int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
    static int8_t readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data);
    static int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
    static int8_t readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data);
    static int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
    static int8_t readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data);
    static int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
    static int8_t readBytesNoRegAddress(uint8_t devAddr, uint8_t length, uint8_t *data);
    static int8_t readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

    static bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
    static bool writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
    static bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
    static bool writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
    static bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
    static bool writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
    static bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
    static bool writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

    static uint16_t readTimeout;
};

#endif /* _I2Cdev_H_ */