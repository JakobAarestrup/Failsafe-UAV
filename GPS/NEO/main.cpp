#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "Neo7.hpp"

int main()
{
/* VARIABLES */
  int serial_port = 0;
  char GPS_data;
  char def_1[] = "0";
  char def_2[] = "N/A";
  char* d1 = def_1;
  char* d2 = def_2;

  float Long = 0.0, Lat = 0.0;
  int NS = 0, EW = 0;
  
  
/* OPEN UART */
  int serial_port = openUart(serial_port); // opens serial port

/* CONFIGURATION */
  config(serial_port,""); // disable..


/* START LOGGING*/
  sendToLogFile(); //Der er 2 måder logger alt eller logger kun når fil bliver kaldt. Det nemmeste er nok alt for at slippe for en klasse hovedpine for alt data.

/* STARTING LOOP*/
  while(1)
    {
      readGPS(serial_port, GPS_data, d1, d2); // reads NMEA message
      
      Long = getLongitude();    // returns longitude
      Lat = getLatitude();      // returns latitude
      NS = getSouthNorth();     // returns either a north pole or south pole
      EW = getEastWest();       // returns either a East pole or West pole

      convertdata(Long, Lat, NS, EW); //converts to decimal degrees format

      usleep(1000000); // delay 1 second
    }

  return 0;

}