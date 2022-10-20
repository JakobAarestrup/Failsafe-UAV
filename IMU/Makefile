#
# Makefile:
#################################################################################


#DEBUG  = -g -O0
DEBUG   = -O3
CC      = gcc
INCLUDE = -I/usr/local/include
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS = -L/usr/local/lib
LDLIBS    = -lwiringPiI2C -lwiringPi -lpthread -lm

# Should not alter anything below this line
###############################################################################

SRC     =       i2ctest3.c                                               \