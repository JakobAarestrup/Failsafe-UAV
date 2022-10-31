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
/*NMEA*/ // disable SBAS QZSS GLONASS BeiDou Galileo
const char NMEA[] = "\xB5\x62\x06\x17\x0C\x00\x00\x23\x00\x02\x76\x00\x00\x00\x00\x00\x00\x00\xC4\xA4";
size_t NMEA_Length = sizeof NMEA / sizeof NMEA[0]; // length of NMEA CFG Message

/*RATE*/ // Measurement frequency: 10 hz, navigation frequency 10 hz
const char RATE[] = "\xB5\x62\x06\x08\x06\x00\x64\x00\x01\x00\x01\x00\x7A\x12"; 
size_t RATE_Length = sizeof RATE / sizeof RATE[0]; // length of RATE CFG Message

/*NMEA MESSAGES*/
const char GLL[] = "\xB5\x62\x06\x01\x08\x00\xF0\x01\x00\x00\x00\x00\x00\x00\x00\x2A"; // disable GPGLL
const char GSA[] = "\xB5\x62\x06\x01\x08\x00\xF0\x02\x00\x00\x00\x00\x00\x00\x01\x31"; // disable GSA
const char GSV[] = "\xB5\x62\x06\x01\x08\x00\xF0\x03\x00\x00\x00\x00\x00\x00\x02\x38"; // disable GPGSV
const char RMC[] = "\xB5\x62\x06\x01\x08\x00\xF0\x04\x00\x00\x00\x00\x00\x00\x03\x3F"; // disable RMC
const char VTG[] = "\xB5\x62\x06\x01\x08\x00\xF0\x05\x00\x00\x00\x00\x00\x00\x04\x46"; // disable VTG
size_t GP_Length = sizeof GLL / sizeof GLL[0]; // length of all NMEA MESSAGES
printf("GP_Length: %d\n", GP_Length);
printf("NMEA_Length: %d\n", NMEA_Length);
printf("RATE_Length: %d\n", RATE_Length);
int serial_port;

    /*freopen ("RDSLog.txt", "w",stdout);
    std::cout << "Hello mister log file" << std::endl;
    return 0;*/
if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)		/* open serial port */
  { // mere baudrate ekstra https://freematics.com/forum/viewtopic.php?t=1759
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

write(serial_port,GLL,GP_Length);
write(serial_port,GSA,GP_Length);
write(serial_port,GSV,GP_Length);
write(serial_port,RMC,GP_Length);
write(serial_port,VTG,GP_Length);

write(serial_port,NMEA, NMEA_Length);
write(serial_port,RATE,RATE_Length);



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

