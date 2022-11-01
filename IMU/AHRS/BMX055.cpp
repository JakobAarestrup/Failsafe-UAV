/*
 * mbed library program
 *  BMX055 Small, versatile 9-axis sensor module
 *  by Bosch Sensortec
 *
 * Copyright (c) 2018,'19 Kenji Arai / JH1PJL
 *  http://www.page.sannet.ne.jp/kenjia/index.html
 *  http://mbed.org/users/kenjiArai/
 *      Started:    October   24th, 2018
 *      Revised:    March      3rd, 2019
 */
 
#include "mbed.h"
#include "BMX055.h"
 
#define DEBUG 0
 
#if MBED_MAJOR_VERSION == 2
#define WAIT_MS(x)       wait_ms(x)
#elif  MBED_MAJOR_VERSION == 5
#define WAIT_MS(x)       Thread::wait(x)
#else
#error "Running on Unknown OS"
#endif
 
BMX055::BMX055 (I2C &i2c) : i2c(i2c)
{
    //this->i2c = &i2c;
    bmx055_parameters = bmx055_std_paramtr;
    initialize ();
}

uint16_t BMX055::begin(void){
    return 1;
    }


/////////////// Set parameter /////////////////////////////
void BMX055::set_parameter(const BMX055_TypeDef *bmx055_parameter)
{
    bmx055_parameters = *bmx055_parameter;
    set_parameters_to_regs();
    
}
 
/////////////// Read data & normalize /////////////////////
void BMX055::readAccel(void)
{
    int16_t x,y,z;
 
    chip_addr = inf_addr.acc_addr;
    dt[0] = 0x02;
    i2c.write(chip_addr, dt, 1, true);
    i2c.read(chip_addr, dt, 6, false);
    x = dt[1] << 8 | (dt[0] & 0xf0); // values have to be divided by 16
    y = dt[3] << 8 | (dt[2] & 0xf0);
    z = dt[5] << 8 | (dt[4] & 0xf0);
    accX = (float)x * acc_factor;
    accY = (float)y * acc_factor;
    accZ = (float)z * acc_factor;
}
 
void BMX055::readGyro(void)
{
    int16_t x,y,z;

    chip_addr = inf_addr.gyr_addr;
    dt[0] = 0x02;
    i2c.write(chip_addr, dt, 1, true);
    i2c.read(chip_addr, dt, 6, false);
    x = dt[1] << 8 | dt[0];
    y = dt[3] << 8 | dt[2];
    z = dt[5] << 8 | dt[4];

    gyroX = (float)x * gyr_factor;
    gyroY = (float)y * gyr_factor;
    gyroZ = (float)z * gyr_factor;
}
 
void BMX055::readMag(void)
{
    int16_t x,y,z;

    chip_addr = inf_addr.mag_addr;
    dt[0] = 0x42;
    i2c.write(chip_addr, dt, 1, true);
    i2c.read(chip_addr, dt, 6, false);
    
    // corrected, pmic 21.01.2020
    x = dt[1] << 8 | (dt[0] & 0xf8); // value has to be divided by 8
    y = dt[3] << 8 | (dt[2] & 0xf8); // value has to be divided by 8
    z = dt[5] << 8 | (dt[4] & 0xfe); // value has to be divided by 2
    
    // scaling factor 1/301.5815f if experimentally validated in comparision to pes board -> approx. same scaling of mag values, pmic 21.01.2020
    magX = (float)y / 8.0f / 301.5815f;       // MAGNETOMETER x-y are switched, see BMX055 datasheet page 161, also measurements show change in sign x!!!
    magY = -(float)x / 8.0f / 301.5815f;       // x-y is switched
    magZ = -(float)z / 2.0f / 301.5815f;       // z is -z
}
 
float BMX055::get_chip_temperature()
{
    chip_addr = inf_addr.acc_addr;
    dt[0] = 0x08;   // chip tempareture reg addr
    i2c.write(chip_addr, dt, 1, true);
    i2c.read(chip_addr, dt, 1, false);
    //printf("Temp reg = 0x%02x\r\n", dt[0]);
    return (float)((int8_t)dt[0]) * 0.5f + 23.0f;
}
 
/////////////// Initialize ////////////////////////////////
void BMX055::initialize (void)
{
    i2c.frequency(400000);
    // Check Acc & Mag & Gyro are available of not
    check_id();
    if (ready_flag == 0x07){
    }
    // Set initial data
    set_parameters_to_regs();
    switch(bmx055_parameters.gyr_fs){
        case GYR_2000DPS:
            gyr_factor = 2000.0f*0.01745329f / 32768.0f;
            break;
        case GYR_1000DPS:
            gyr_factor = 1000.0f*0.01745329f / 32768.0f;
            break;
        case GYR_500DPS:
            gyr_factor = 500.0f*0.01745329f / 32768.0f;
            break;
        case GYR_250DPS:
            gyr_factor = 250.0f*0.01745329f / 32768.0f;
            break;
        case GYR_125DPS:
            gyr_factor = 125.0f*0.01745329f / 32768.0f;
            break;
        default:
            gyr_factor = 0;
            break;
    }
    switch(bmx055_parameters.acc_fs){
        case ACC_2G:
            acc_factor = 2.0f * 9.81f / 2048.0f / 16.0f;
            break;
        case ACC_4G:
            acc_factor = 4.0f * 9.81f / 2048.0f / 16.0f;
            break;
        case ACC_8G:
            acc_factor = 8.0f * 9.81f / 2048.0f / 16.0f;
            break;
        case ACC_16G:
            acc_factor = 16.0f * 9.81f / 2048.0f / 16.0f;
            break;
        default:
            acc_factor = 0;
            break;
    }


}
 
////// Set initialize data to related registers ///////////
void BMX055::set_parameters_to_regs(void)
{
    // ACC
    chip_addr = inf_addr.acc_addr;
    dt[0] = 0x0f;   // Select PMU_Range register
    dt[1] = bmx055_parameters.acc_fs;
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x10;   // Select PMU_BW register
    dt[1] = bmx055_parameters.acc_bw;
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x11;   // Select PMU_LPW register
    dt[1] = 0x00;   // Normal mode, Sleep duration = 0.5ms
    i2c.write(chip_addr, dt, 2, false);
    // GYR
    chip_addr = inf_addr.gyr_addr;
    dt[0] = 0x0f;   // Select Range register
    dt[1] = bmx055_parameters.gyr_fs;
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x10;   // Select Bandwidth register
    dt[1] = bmx055_parameters.gyr_bw;
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x11;   // Select LPM1 register
    dt[1] = 0x00;   // Normal mode, Sleep duration = 2ms
    i2c.write(chip_addr, dt, 2, false);
    // MAG
    chip_addr = inf_addr.mag_addr;
    dt[0] = 0x4b;   // Select Mag register
    dt[1] = 0x83;   // Soft reset 10000011
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(10);
    dt[0] = 0x4b;   // Select Mag register
    dt[1] = 0x01;   // Soft reset 00000001
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(10);
    dt[0] = 0x4c;   // Select Mag register
    dt[1] = bmx055_parameters.mag_odr << 3; // corrected, pmic 20.01.2020
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x4e;   // Select Mag register
    dt[1] = 0x84;   // X, Y, Z-Axis enabled
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x51;   // Select Mag register
    dt[1] = 0x51;   // No. of Repetitions for X-Y Axis = 163, fmaxODR = 33.2060 Hz, pmic 21.01.2020
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x52;   // Select Mag register
    dt[1] = 0x0a;   // No. of Repetitions for Z-Axis = 11, fmaxODR = 33.2060 Hz, pmic 21.01.2020
    i2c.write(chip_addr, dt, 2, false);
#if 0
    // ACC
    chip_addr = inf_addr.acc_addr;
    dt[0] = 0x0f;   // Select PMU_Range register
    dt[1] = 0x03;   // Range = +/- 2g
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x10;   // Select PMU_BW register
    dt[1] = 0x08;   // Bandwidth = 7.81 Hz
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x11;   // Select PMU_LPW register
    dt[1] = 0x00;   // Normal mode, Sleep duration = 0.5ms
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    // GYR
    chip_addr = inf_addr.gyr_addr;
    dt[0] = 0x0f;   // Select Range register
    dt[1] = 0x04;   // Full scale = +/- 125 degree/s
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x10;   // Select Bandwidth register
    dt[1] = 0x07;   // ODR = 100 Hz
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x11;   // Select LPM1 register
    dt[1] = 0x00;   // Normal mode, Sleep duration = 2ms
    i2c.write(chip_addr, dt, 2, false);
    // MAG
    chip_addr = inf_addr.mag_addr;
    dt[0] = 0x4b;   // Select Mag register
    dt[1] = 0x83;   // Soft reset
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(10);
    dt[0] = 0x4b;   // Select Mag register
    dt[1] = 0x01;   // Soft reset
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(10);
    dt[0] = 0x4c;   // Select Mag register
    dt[1] = 0x00;   // Normal Mode, ODR = 10 Hz
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x4e;   // Select Mag register
    dt[1] = 0x84;   // X, Y, Z-Axis enabled
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x51;   // Select Mag register
    dt[1] = 0x04;   // No. of Repetitions for X-Y Axis = 9
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
    dt[0] = 0x52;   // Select Mag register
    dt[1] = 0x16;   // No. of Repetitions for Z-Axis = 15
    i2c.write(chip_addr, dt, 2, false);
    wait_ms(1);
#endif
}
 
/////////////// Check Who am I? ///////////////////////////
void BMX055::check_id(void)
{
    ready_flag = 0;
    // ID ACC
    inf_addr.acc_addr = BMX055_ACC_CHIP_ADDR;
    chip_addr = inf_addr.acc_addr;
    dt[0] = 0x00;   // chip ID reg addr
    i2c.write(chip_addr, dt, 1, true);
    i2c.read(chip_addr, dt, 1, false);
    inf_id.acc_id = dt[0];
    if (inf_id.acc_id == I_AM_BMX055_ACC) {
        ready_flag |= 0x01;
    } else {
        inf_addr.acc_addr = (0x18 << 1);
        chip_addr = inf_addr.acc_addr;
        dt[0] = 0x00;   // chip ID reg addr
        i2c.write(chip_addr, dt, 1, true);
        i2c.read(chip_addr, dt, 1, false);
        inf_id.acc_id = dt[0];
        if (inf_id.acc_id == I_AM_BMX055_ACC) {
            ready_flag |= 0x01;
        }
    }
    // ID GYRO
    inf_addr.gyr_addr = BMX055_GYR_CHIP_ADDR;
    chip_addr = inf_addr.gyr_addr;
    dt[0] = 0x00;   // chip ID reg addr
    i2c.write(chip_addr, dt, 1, true);
    i2c.read(chip_addr, dt, 1, false);
    inf_id.gyr_id = dt[0];
    if (inf_id.gyr_id == I_AM_BMX055_GYR) {
        ready_flag |= 0x02;
    } else {
        inf_addr.gyr_addr = (0x68 << 1);
        chip_addr = inf_addr.gyr_addr;
        dt[0] = 0x00;   // chip ID reg addr
        i2c.write(chip_addr, dt, 1, true);
        i2c.read(chip_addr, dt, 1, false);
        inf_id.gyr_id = dt[0];
        if (inf_id.gyr_id == I_AM_BMX055_GYR) {
            ready_flag |= 0x02;
        }
    }
    // ID Mag
    inf_addr.mag_addr = BMX055_MAG_CHIP_ADDR;
    chip_addr = inf_addr.mag_addr;
    dt[0] = 0x4b;   // reg addr
    dt[1] = 0x01;   // control power bit set 1
    i2c.write(chip_addr, dt, 2, false);
    dt[0] = 0x40;   // chip ID reg addr
    i2c.write(chip_addr, dt, 1, true);
    i2c.read(chip_addr, dt, 1, false);
    inf_id.mag_id = dt[0];
    if (inf_id.mag_id == I_AM_BMX055_MAG) {
        ready_flag |= 0x04;
    } else {
        inf_addr.mag_addr = (0x12 << 1);
        chip_addr = inf_addr.mag_addr;
        // control power bit set 1
        dt[0] = 0x4b;
        dt[1] = 0x01;
        i2c.write(chip_addr, dt, 2, false);
        dt[0] = 0x40;
        i2c.write(chip_addr, dt, 1, true);
        i2c.read(chip_addr, dt, 1, false);
        inf_id.mag_id = dt[0];
        if (inf_id.mag_id == I_AM_BMX055_MAG) {
            ready_flag |= 0x04;
        } else {
            inf_addr.mag_addr = (0x11 << 1);
            chip_addr = inf_addr.mag_addr;
            // control power bit set 1
            dt[0] = 0x4b;
            dt[1] = 0x01;
            i2c.write(chip_addr, dt, 2, false);
            dt[0] = 0x40;
            i2c.write(chip_addr, dt, 1, true);
            i2c.read(chip_addr, dt, 1, false);
            inf_id.mag_id = dt[0];
            if (inf_id.mag_id == I_AM_BMX055_MAG) {
                ready_flag |= 0x04;
            } else {
                inf_addr.mag_addr = (0x10 << 1);
                chip_addr = inf_addr.mag_addr;
                // control power bit set 1
                dt[0] = 0x4b;
                dt[1] = 0x01;
                i2c.write(chip_addr, dt, 2, false);
                dt[0] = 0x40;
                i2c.write(chip_addr, dt, 1, true);
                i2c.read(chip_addr, dt, 1, false);
                inf_id.mag_id = dt[0];
                if (inf_id.mag_id == I_AM_BMX055_MAG) {
                    ready_flag |= 0x04;
                }
            }
        }
    }
#if DEBUG
    printf("ACC addr=0x%02x, id=0x%02x\r\n", chip_addr, inf_id.acc_id); 
    printf("GYR addr=0x%02x, id=0x%02x\r\n", chip_addr, inf_id.gyr_id); 
    printf("MAG addr=0x%02x, id=0x%02x\r\n", chip_addr, inf_id.mag_id); 
    printf("ready_flag = 0x%x\r\n", ready_flag);
#endif
}
 
void BMX055::read_id_inf(BMX055_ID_INF_TypeDef *id)
{
    id->acc_id = acc_id;
    id->mag_id = mag_id;
    id->gyr_id = gyr_id;
}
 
/////////////// Check chip ready or not  //////////////////
bool BMX055::chip_ready(void)
{
    if (ready_flag == 0x07) {
        return true;
    }
    return false;
}
 
/////////////// I2C Freq. /////////////////////////////////
void BMX055::frequency(int hz)
{
    i2c.frequency(hz);
}
 
/////////////// Read/Write specific register //////////////
uint8_t BMX055::read_reg(uint8_t addr)
{
    dt[0] = addr;
    i2c.write(chip_addr, dt, 1, true);
    i2c.read(chip_addr, dt, 1, false);
    return (uint8_t)dt[0];
}
 
uint8_t BMX055::write_reg(uint8_t addr, uint8_t data)
{
    uint8_t d;
 
    dt[0] = addr;
    dt[1] = data;
    i2c.write(chip_addr, dt, 2, false);
    d = dt[0];
    return d;
}