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
const char GLL[] = "\xB5\x62\x06\x01\x08\x00\xF0\x01\x00\x00\x00\x00\x00\x00\x00\x2A"; // disable GPGLL
const char GSA[] = "\xB5\x62\x06\x01\x08\x00\xF0\x02\x00\x00\x00\x00\x00\x00\x01\x31"; // disable GSA
const char GSV[] = "\xB5\x62\x06\x01\x08\x00\xF0\x03\x00\x00\x00\x00\x00\x00\x02\x38"; // disable GPGSV
const char RMC[] = "\xB5\x62\x06\x01\x08\x00\xF0\x04\x00\x00\x00\x00\x00\x00\x03\x3F"; // disable RMC
const char VTG[] = "\xB5\x62\x06\x01\x08\x00\xF0\x05\x00\x00\x00\x00\x00\x00\x04\x46"; // disable VTG
size_t GP_Length = sizeof GLL / sizeof GLL[0];

int serial_port;

    /*freopen ("RDSLog.txt", "w",stdout);
    std::cout << "Hello mister log file" << std::endl;
    return 0;*/
if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)		/* open serial port */
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }
  
write(serial_port,GLL,GP_Length);
write(serial_port,GSA,GP_Length);
write(serial_port,GSV,GP_Length);
write(serial_port,RMC,GP_Length);
write(serial_port,GLL,GP_Length);


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

