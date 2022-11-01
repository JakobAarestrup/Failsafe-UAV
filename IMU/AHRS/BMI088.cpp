#include "BMI088.h"

BMI088::BMI088(I2C& i2c)
{
    this->i2c = &i2c;
    devAddrAcc = BMI088_ACC_ADDRESS;
    devAddrGyro = BMI088_GYRO_ADDRESS;
    initialize();
}

void BMI088::initialize(void)
{
    setAccScaleRange(RANGE_3G);
    wait(.02);
    setAccOutputDataRate(ODR_50);  // ODR_50 -> 20 Hz bandwith in normal mode
    // setAccOutputDataRateOSR2(ODR_100); // ODR_100 -> 19 Hz bandwith in OSR2 mode
    wait(.02);
    setAccPowerMode(ACC_ACTIVE);
    wait(.02);
    setGyroScaleRange(RANGE_500);
    wait(.02);
    setGyroOutputDataRate(ODR_200_BW_23);
    wait(.02);
    setGyroPowerMode(GYRO_NORMAL);
}

bool BMI088::isConnection(void)
{
    return ((getAccID() == 0x1E) && (getGyroID() == 0x0F));
}

void BMI088::resetAcc(void)
{
    write8(ACC, BMI088_ACC_SOFT_RESET, 0xB6);
}

void BMI088::resetGyro(void)
{
    write8(GYRO, BMI088_GYRO_SOFT_RESET, 0xB6);
}

uint8_t BMI088::getAccID(void)
{
    return read8(ACC, BMI088_ACC_CHIP_ID);
}

uint8_t BMI088::getGyroID(void)
{
    return read8(GYRO, BMI088_GYRO_CHIP_ID);
}

void BMI088::setAccPowerMode(acc_power_type_t mode)
{
    if(mode == ACC_ACTIVE) {
        write8(ACC, BMI088_ACC_PWR_CTRl, 0x04);
        write8(ACC, BMI088_ACC_PWR_CONF, 0x00);
    } else if(mode == ACC_SUSPEND) {
        write8(ACC, BMI088_ACC_PWR_CONF, 0x03);
        write8(ACC, BMI088_ACC_PWR_CTRl, 0x00);
    }
}

void BMI088::setGyroPowerMode(gyro_power_type_t mode)
{
    if(mode == GYRO_NORMAL) {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_NORMAL);
    } else if(mode == GYRO_SUSPEND) {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_SUSPEND);
    } else if(mode == GYRO_DEEP_SUSPEND) {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_DEEP_SUSPEND);
    }
}

void BMI088::setAccScaleRange(acc_scale_type_t range)
{
    if(range == RANGE_3G) accRange = 3.0f * 9.81f / 32768.0f;
    else if(range == RANGE_6G) accRange = 6.0f * 9.81f / 32768.0f;
    else if(range == RANGE_12G) accRange = 12.0f * 9.81f / 32768.0f;
    else if(range == RANGE_24G) accRange = 24.0f * 9.81f / 32768.0f;

    write8(ACC, BMI088_ACC_RANGE, (uint8_t)range);
}

void BMI088::setAccOutputDataRate(acc_odr_type_t odr)
{
    /*
    uint8_t data = 0;

    data = read8(ACC, BMI088_ACC_CONF);
    data = data & 0xf0; // 0xf0 := 11110000
    data = data | (uint8_t)odr;

    write8(ACC, BMI088_ACC_CONF, data);
    */
    
    uint8_t data = (uint8_t)0xa0 | (uint8_t)odr;
    write8(ACC, BMI088_ACC_CONF, data);
}

void BMI088::setAccOutputDataRateOSR2(acc_odr_type_t odr)
{
    uint8_t data = (uint8_t)0x90 | (uint8_t)odr;
    write8(ACC, BMI088_ACC_CONF, data);
}

void BMI088::setAccOutputDataRateOSR4(acc_odr_type_t odr)
{
    uint8_t data = (uint8_t)0x80 | (uint8_t)odr;
    write8(ACC, BMI088_ACC_CONF, data);
}

void BMI088::setGyroScaleRange(gyro_scale_type_t range)
{
    if(range == RANGE_2000) gyroRange = 2000.0f * 0.01745329f / 32768.0f;
    else if(range == RANGE_1000) gyroRange = 1000.0f * 0.01745329f / 32768.0f;
    else if(range == RANGE_500) gyroRange = 500.0f * 0.01745329f / 32768.0f;
    else if(range == RANGE_250) gyroRange = 250.0f * 0.01745329f / 32768.0f;
    else if(range == RANGE_125) gyroRange = 125.0f * 0.01745329f / 32768.0f;

    write8(GYRO, BMI088_GYRO_RANGE, (uint8_t)range);
}

void BMI088::setGyroOutputDataRate(gyro_odr_type_t odr)
{
    write8(GYRO, BMI088_GYRO_BAND_WIDTH, (uint8_t)odr);
}

void BMI088::getAcceleration(float* x, float* y, float* z)
{
    uint8_t buf[6] = {0};
    uint16_t ax = 0, ay = 0, az = 0;
    float value = 0;

    read(ACC, BMI088_ACC_X_LSB, buf, 6);

    ax = buf[0] | (buf[1] << 8);
    ay = buf[2] | (buf[3] << 8);
    az = buf[4] | (buf[5] << 8);

    value = (int16_t)ax;
    *x = accRange * value ;

    value = (int16_t)ay;
    *y = accRange * value;

    value = (int16_t)az;
    *z = accRange * value ;
}

float BMI088::getAccelerationX(void)
{
    uint16_t ax = 0;
    float value = 0;

    ax = read16(ACC, BMI088_ACC_X_LSB);

    value = (int16_t)ax;
    value = accRange * value ;

    return value;
}

float BMI088::getAccelerationY(void)
{
    uint16_t ay = 0;
    float value = 0;

    ay = read16(ACC, BMI088_ACC_Y_LSB);

    value = (int16_t)ay;
    value = accRange * value ;

    return value;
}

float BMI088::getAccelerationZ(void)
{
    uint16_t az = 0;
    float value = 0;

    az = read16(ACC, BMI088_ACC_Z_LSB);

    value = (int16_t)az;
    value = accRange * value;

    return value;
}
void BMI088::readAccel(void){
    accX = getAccelerationX();
    accY = getAccelerationY();
    accZ = getAccelerationZ();
    }
    

void BMI088::getGyroscope(float* x, float* y, float* z)
{
    uint8_t buf[6] = {0};
    uint16_t gx = 0, gy = 0, gz = 0;
    float value = 0;

    read(GYRO, BMI088_GYRO_RATE_X_LSB, buf, 6);

    gx = buf[0] | (buf[1] << 8);
    gy = buf[2] | (buf[3] << 8);
    gz = buf[4] | (buf[5] << 8);

    value = (int16_t)gx;
    *x = gyroRange * value ;

    value = (int16_t)gy;
    *y = gyroRange * value ;

    value = (int16_t)gz;
    *z = gyroRange * value ;
}

float BMI088::getGyroscopeX(void)
{
    uint16_t gx = 0;
    float value = 0;

    gx = read16(GYRO, BMI088_GYRO_RATE_X_LSB);
    value = (int16_t)gx;
    value = gyroRange * value;
    return value;
}

float BMI088::getGyroscopeY(void)
{
    uint16_t gy = 0;
    float value = 0;

    gy = read16(GYRO, BMI088_GYRO_RATE_Y_LSB);

    value = (int16_t)gy;
    value = gyroRange * value ;

    return value;
}

float BMI088::getGyroscopeZ(void)
{
    uint16_t gz = 0;
    float value = 0;

    gz = read16(GYRO, BMI088_GYRO_RATE_Z_LSB);

    value = (int16_t)gz;
    value = gyroRange * value ;

    return value;
}
void BMI088::readGyro(void){
    gyroX = getGyroscopeX();
    gyroY = getGyroscopeY();
    gyroZ = getGyroscopeZ();
    }
int16_t BMI088::getTemperature(void)
{
    int16_t data = 0;

    data = read16Be(ACC, BMI088_ACC_TEMP_MSB);
    data = data >> 5;

    if(data > 1023) data = data - 2048;

    return (int16_t)(data / 8 + 23);
}

void BMI088::write8(device_type_t dev, uint8_t reg, uint8_t val)
{
    uint8_t addr = 0;

    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    i2c->start();
    if(i2c->write(addr << 1 | 0) != 1) {
        return;
    }
    if(i2c->write(reg) != 1 ) {
        return;
    }
    if(i2c->write(val) != 1 ) {
        return;
    }
    i2c->stop();


}

uint8_t BMI088::read8(device_type_t dev, uint8_t reg)
{
    uint8_t addr = 0, data = 0;

    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    i2c->start();
    if(i2c->write(addr << 1 | 0) != 1) {
        return 0xff;
    }

    if(i2c->write(reg)!=1) {
        return 0xff;
    }
    //i2c->stop();


    i2c->start();
    if(i2c->write(addr<<1|1)!=1) {
        return 0xff;
    }

    data = i2c->read(0);
    i2c->stop();

    return data;
}

uint16_t BMI088::read16(device_type_t dev, uint8_t reg)
{
    uint8_t addr = 0;
    uint16_t msb = 0, lsb = 0;

    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    i2c->start();
    if(i2c->write(addr << 1 | 0) != 1) {
        return 0xffff;
    }

    if(i2c->write(reg) != 1) {
        return 0xffff;
    }
    //i2c->stop();


    i2c->start();
    if(i2c->write(addr << 1 | 1) != 1) {
        return 0xffff;
    }

    //i2c->start();
    lsb = i2c->read(1);
    msb = i2c->read(0);
    i2c->stop();

    return (lsb | (msb << 8));
}

// bei der Temp sind MSB und LSB vertauscht
uint16_t BMI088::read16Be(device_type_t dev, uint8_t reg)
{
    uint8_t addr = 0;
    uint16_t msb = 0, lsb = 0;

    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    i2c->start();
    if(i2c->write(addr << 1 | 0) != 1) {
        return 0xffff;
    }

    if(i2c->write(reg) != 1) {
        return 0xffff;
    }
    //i2c->stop();


    i2c->start();
    if(i2c->write(addr << 1 | 1) != 1) {
        return 0xffff;
    }

    msb = i2c->read(1);
    lsb = i2c->read(0);
    i2c->stop();

    return (lsb | (msb << 8));
}

uint32_t BMI088::read24(device_type_t dev, uint8_t reg)
{
    uint8_t addr = 0;
    uint32_t hsb = 0, msb = 0, lsb = 0;

    i2c->start();
    if(i2c->write(addr<<1|0)!=1) {
        return 0xffffff;
    }

    if(i2c->write(reg)!=1) {
        return 0xffffff;
    }
    i2c->stop();


    i2c->start();
    if(i2c->write(addr<<1|1)!=1) {
        return 0xffffff;
    }

    lsb = i2c->read(1);
    msb = i2c->read(1);
    hsb = i2c->read(0);
    i2c->stop();

    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    return (lsb | (msb << 8) | (hsb << 16));
}

void BMI088::read(device_type_t dev, uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t addr = 0;

    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    i2c->start();
    if(i2c->write(addr<<1|0)!=1) {
        for(int in(0); in < len; in++) {
            buf[in] = 0xff;
        }
        return;
    }
    
    if(i2c->write(reg)!=1) {/*
        for(int in(0); in < len; in++) {
            buf[in] = 0xff;
        }
        return;*/
    }
    i2c->stop();


    i2c->start();
    if(i2c->write(addr<<1|1)!=1) {
        for(int in(0); in < len; in++) {
            buf[in] = 0xff;
        }
        return;
    }

    for(int in = 0; in < len; in++) {
        buf[in] = i2c->read(1);
    }
    i2c->stop();
    return;

}

//BMI088
