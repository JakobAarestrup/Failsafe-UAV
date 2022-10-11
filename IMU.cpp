#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>

#define I2C_DEVICE        "/dev/i2c-1"
#define LSM6DS0X_ADDR     (0x6a)
#define LIS3MDL_ADDR      (0x1c)

int fs;
int ret;
uint8_t buff[4];
uint8_t looper;

static float convert_reading_to_temp(uint16_t reading) {
    // As per datasheet:
    // 1 LSB = 0.03125 deg C (1/32)
    // Process: Right shift 16-bit value by 2 and then /32

    float act_temp;
    reading = reading >> 2;
    act_temp = (float)((float)reading / 32);
    return act_temp;

int main(void) {

    fd = open(I2C_DEVICE, O_RDWR);
    if(fd < 0) {
        printf("Could not open the I2C device...\r\n");
        exit(EXIT_FAILURE);
    }

    if(ioctl(fd,I2C_SLAVE,TMP006_ADDR) < 0) {
        printf("Could not set I2C device address...\r\n");
        exit(EXIT_FAILURE);
    }
/////////////////////
    buff[0] = TMP006_PTR_MANID;
    if(write(fd,buff,1) != 1) {
        printf("Could not write the pointer register...\r\n");
        exit(EXIT_FAILURE);
    }

    if(read(fd,buff,2) != 2) {
        printf("Could not read the MAN ID...\r\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("The MAN ID is: %02x%02x...\r\n",buff[0],buff[1]);
    }
/////////////////////
    buff[0] = TMP006_PTR_DEVID;
    if(write(fd,buff,1) != 1) {
        printf("Could not write the pointer register...\r\n");
        exit(EXIT_FAILURE);
    }

    if(read(fd,buff,2) != 2) {
        printf("Could not read the DEV ID...\r\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("The DEV ID is: %02x%02x...\r\n",buff[0],buff[1]);
    }
/////////////////////
    buff[0] = TMP006_PTR_CFG;
    if(write(fd,buff,1) != 1) {
        printf("Could not write the pointer register...\r\n");
        exit(EXIT_FAILURE);
    }

    if(read(fd,buff,2) != 2) {
        printf("Could not read the Config Register...\r\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("The Config is: %02x%02x...\r\n",buff[0],buff[1]);
    }
/////////////////////
    buff[0] = TMP006_PTR_CFG;
    buff[1] = 0x70;
    buff[2] = 0x80;

    if(write(fd,buff,3) != 3) {
        printf("Could not write the pointer register...\r\n");
        exit(EXIT_FAILURE);
    }
    sleep(1);
/////////////////////
    buff[0] = TMP006_PTR_CFG;
    if(write(fd,buff,1) != 1) {
        printf("Could not write the pointer register...\r\n");
        exit(EXIT_FAILURE);
    }

    if(read(fd,buff,2) != 2) {
        printf("Could not read the modified Config Register...\r\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("The modified Config is: %02x%02x...\r\n",buff[0],buff[1]);
    }
/////////////////////
    for(looper=0; looper<20; ++looper) {
        usleep(1000000);
        buff[0] = TMP006_PTR_TEMP;
        if(write(fd,buff,1) != 1) {
            printf("Could not write the pointer register...\r\n");
            exit(EXIT_FAILURE);
        }

        if(read(fd,buff,2) != 2) {
            printf("Could not read the TEMPERATURE...\r\n");
            exit(EXIT_FAILURE);
        }
        else {
            temp_reading = buff[0];
            temp_reading <<= 8;
            temp_reading |= buff[1];
            printf("The TEMPERATURE is: %.2f...\r\n",convert_reading_to_temp(temp_reading));
        }

    }

    close(fd);

    exit(EXIT_SUCCESS);