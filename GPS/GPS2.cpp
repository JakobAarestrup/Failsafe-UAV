#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int serial_port; 
  char dat,buff[100],GGA_code[3];
  unsigned char IsitGGAstring=0;
  unsigned char GGA_index=0;
  unsigned char is_GGA_received_completely = 0;
  int length = 0;
  int count = 0;

  
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

  while(1){
	  
		if(serialDataAvail (serial_port) )		/* check for any data available on serial port */
		  { 
			dat = serialGetchar(serial_port);		/* receive character serially */		
			if(dat == '$')
      {
				IsitGGAstring = 0;
				GGA_index = 0;
			}
			else if(IsitGGAstring ==1){
				buff[GGA_index++] = dat;
          if(dat=='\r')
            is_GGA_received_completely = 1;
          }
			else if(GGA_code[0]=='G' && GGA_code[1]=='G' && GGA_code[2]=='A'){
				IsitGGAstring = 1;
				GGA_code[0]= 0; 
				GGA_code[0]= 0;
				GGA_code[0]= 0;		
				}
			else{
				GGA_code[0] = GGA_code[1];
				GGA_code[1] = GGA_code[2];
				GGA_code[2] = dat;
				}
		  }
		if(is_GGA_received_completely==1){
			//printf("GPGGA: %s",buff);
      char* NMEA[15];
      char* b1 = strtok(buff,","); // first token
      while (b1 != NULL)
      {
          NMEA[count++] = b1; 
          b1 = strtok(NULL, ",");
          
      }
      char* latitude = NMEA[2];
      char* longitude = NMEA[4];
      char* Pole_NS = NMEA[3];
      char* Pole_WE = NMEA[5];        
      char* satellites = NMEA[7];

      // conversion
      float latitude_ = atof(latitude);
      float longitude_ = atof(longitude);
      int satellites_ = atoi(satellites);

      printf("NMEA: %f %s %f %s %d",latitude_,Pole_NS,longitude_,Pole_WE,satellites_);
      char* b1 = 0;
      count = 0;
			is_GGA_received_completely = 0;
		}
	}
	return 0;
}