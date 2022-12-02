#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <cstddef>
#include <unistd.h>

/*Ownlibraries*/
#include "UBX_Protocol_Constants.hpp"
#include "Neo7.hpp"

int main()
{
  /* VARIABLES */
  char GPS_data;
  /* Hvis default values skulle laves char def_1[] = "0";
  char def_2[] = "N/A";

  char* d1 = def_1;
  char* d2 = def_2; */

  float Long = 0.0, Lat = 0.0;

  int serial_port;
  char NS[1];
  char EW[1];

  GPS NEO1;

  /* OPEN UART */
  serial_port = NEO1.configAll(serial_port);
  /* CONFIGURATION */
  printf("Serial port is open! %d \n", serial_port);

  printf("Configuration is done! \n");

  printf("STARTING LOOP\n");
  /* STARTING LOOP*/
  while (1)
  {

    NEO1.readGPS();     // reads NMEA message
    NEO1.convertData(); // converts to decimal degrees format

    usleep(1000000); // delay 1 second
  }

  return 0;
}