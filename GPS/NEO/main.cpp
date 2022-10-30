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
  char def_1[] = "0";
  char def_2[] = "N/A";
  char* d1 = def_1;
  char* d2 = def_2;
  float Long=0.0;
  float Lat=0.0;

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
      readGPS(serial_port, GPS_data, d1 , d2); // reads NMEA message
      Long = getLongitude();    // returns longitude
      Lat = getLatitude();      // returns latitude
      NS = getSouthNorth();     // returns either a north pole or south pole
      EW = getEastWest();       // returns either a East pole or West pole
      convertdata(Long,Lat,NS,EW); //converts to decimal degrees format
      sendToLogFile(); // sends data to log file
      usleep(1000000); // delay 1 second
    }

  return 0;

}