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

const std::byte DataStream[] = 
{
  0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A,
};
const int DataStreamLength = sizeof DataStream / sizeof DataStream[0];

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
 
  serial_port.write(DataStream, DataStreamLength);

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

