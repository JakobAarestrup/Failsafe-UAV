#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "Neo7.hpp"

int main()
{
/* VARIABLES */
  int serial_port;
  char GPS_data;
  GPS G1;
  
/* OPEN UART */
  int serial_port = openUart(serial_port); // opens serial port

/* CONFIGURATION */
  //config("CFG-CFG") // disable..
  //config("CFG-CFG") // disable..
  //config("CFG-CFG") // disable..
  //config("CFG-CFG") // disable..

  //config("CFG-CFG") // set navigation rate to 10 Hz

/* STARTING LOOP*/
  while(1)
    {
      
    usleep(1000000); // delay 1 second
    }

  return 0;

}