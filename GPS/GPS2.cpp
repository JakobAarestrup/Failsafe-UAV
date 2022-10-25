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
  char arr[]="$GPGGA";
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

  while(1){
	int i = 0; 
	if(serialDataAvail (serial_port))
	{ 
		data = serialGetchar (serial_port);		// receive character serially
    // kig efter GPGGA
    if(serialGetchar(serial_port)=='G')
    {
      for(i=0; i<6 ;i++)
      {
        if(serialGetchar(serial_port)==arr[i])
          {
            flag++;
          }
            if (flag == 5)
              {
                flag = 0;
              }
      }
    }

    if(flag==6)
    {
      flag=0;
        for(i=0;i<=65;i++)
        gps[i]=serialGetchar(serial_port);
    }
   
    size_t n = sizeof(gps)/sizeof(gps[0]);
 
    // loop through the array elements
    for (size_t i = 0; i < n; i++) 
    {
      std::cout << gps[i] << ' ';
    }
		//serialPutchar(serial_port, dat); // transmit character serially on port
  
    usleep(1000000);
  }

	}

}