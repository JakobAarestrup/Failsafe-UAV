#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int serial_port ;
  char data;
  int flag = 0;
  char gps[65];
  char arr[]="GPGGA";
  if ((serial_port = serialOpen ("/dev/ttyS0", 9600 )) < 0)	/* open serial port */
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)					/* initializes wiringPi setup */
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  while(1)
  {

    int i = 0; 
    char c;

    if(serialDataAvail (serial_port))
    { 
      data = serialGetchar (serial_port);		// receive character serially
      // kig efter GPGGA
      if(c=serialGetchar(serial_port)=="G")
      {
        for(i=0; i<5 ;i++)
        {
          if(serialGetchar(serial_port)==arr[i])
            {
              flag++;
            }
              if (flag == 4)
                {
                  flag = 0;
                }
        }
      }

      if(flag==5)
      {
        flag=0;
          for(i=0;i<=65;i++)
          gps[i]=serialGetchar(serial_port);
      }
    
      printf("Value of array: %.*s\n", (int)sizeof(gps), gps);
      //serialPutchar(serial_port, dat); // transmit character serially on port
    
      usleep(1000000);
    }

	}

}