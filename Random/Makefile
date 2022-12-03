obj-m := softuart.o
PWD := $(shell pwd)

KERNEL_SRC_DIR := /usr/lib/arm-linux-gnueabihf
GCC_PREFIX := arm-bcmrpi-linux-gnueabihf-

all:
	$(MAKE) -C $(KERNEL_SRC_DIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=$(GCC_PREFIX)

load:
	insmod softuart.ko

unload:
	rmmod softuart