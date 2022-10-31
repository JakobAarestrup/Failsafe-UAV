#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>


int main ()
{
char dat;
int serial_port;

    /*freopen ("RDSLog.txt", "w",stdout);
    std::cout << "Hello mister log file" << std::endl;
    return 0;*/
if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)		/* open serial port */
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

if (wiringPiSetup () == -1)							/* initializes wiringPi setup */
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
 
  serialPuts(serial_port,"B5 62 06 01 08 00 F0 01 00 00 00 00 00 00 00 2A");

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

