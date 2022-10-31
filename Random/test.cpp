#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <cstddef>


int main ()
{
char dat;
const char GLL[] = "\xB5\x62\x06\x01\x08\x00\xF0\x01\x00\x00\x00\x00\x00\x00\x00\x2A";
size_t GLL_Length = sizeof GLL / sizeof GLL[0];
printf("Length of GLL: %d\n", GLL_Length);
int serial_port;

    /*freopen ("RDSLog.txt", "w",stdout);
    std::cout << "Hello mister log file" << std::endl;
    return 0;*/
if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)		/* open serial port */
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }
    write(serial_port,GLL,GLL_Length);

if (wiringPiSetup () == -1)							/* initializes wiringPi setup */
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
 

  while(1)
  {
	  
		if(serialDataAvail(serial_port))		/* check for any data available on serial port */
		  { 
			dat = serialGetchar(serial_port); /* receive character serially */
            printf("%c",dat);
            fflush(stdout);
          }

  }
    return 0;
}

