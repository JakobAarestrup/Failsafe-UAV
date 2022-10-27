#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int serial_port; 
  char dat,buff[100],GGA_code[3];
  unsigned char IsitGGAstring=0;
  unsigned char GGA_index=0;
  unsigned char is_GGA_received_completely = 0;
  float latitude_  = 0;
  float longitude_ = 0;
  int satellites_ = 0;
  int length = 0;
  int count = 0;
  char def_1[] = "0";
  char def_2[] = "N/A";
  char* d1 = def_1;
  char* d2 = def_2;
  int i = 0;
  char* remaining;
  char* NMEA[15]; // array for ASCII tokens
  
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
		if(is_GGA_received_completely==1)
    {
      char* b1 = strtok(buff,","); // first token
      for(i = 0 ; i < 15 ; i++)
      {
          if ( b1 == NULL)
          {
          NMEA[count] = d1; // 0 
          }
          else if (b1 == NULL && count == 3 || 5)
          {
          NMEA[count] = d2; // N/A
          }
          else
          {
          NMEA[count] = b1; 
          }
        count++; // increment
        b1 = strtok(NULL, ",");   
      }

      char* Pole_NS = NMEA[3];
      char* Pole_WE = NMEA[5];

      //conversion
      latitude_   = atof(NMEA[2]);
      longitude_  = atof(NMEA[4]);
      satellites_ = atoi(NMEA[7]);
      printf("GGA: %s\n",buff);
      printf("NMEA: Latitude: %f %s Longitude: %f %s SV: %d\n",latitude_,Pole_NS,longitude_,Pole_WE,satellites_);
      i = 0; // reset i variable
      count = 0; 
			is_GGA_received_completely = 0;
		}
	}
	return 0;
}