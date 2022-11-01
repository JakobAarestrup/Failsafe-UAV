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
  char def_1[] = "0";
  char def_2[] = "N/A";
  GPS NEO1;
  char* d1 = def_1;
  char* d2 = def_2;

  float Long = 0.0, Lat = 0.0;

  int serial_port = 0;
  char* NS, EW;

  
  
/* OPEN UART */
  serial_port = NEO1.openUART(serial_port););

/* CONFIGURATION */
  
  /*NMEA Config*/
  NEO1.config(serial_port, UBX_protocol::NMEA_CFG, UBX_protocol::NMEA_CFG_Length); // disable SBAS QZSS GLONASS BeiDou Galileo
  
  /*Update Rate*/
  NEO1.config(serial_port, UBX_protocol::RATE, UBX_protocol::RATE_Length); // Measurement frequency: 10 hz, navigation frequency 10 hz
 
  /*NMEA messages*/
  NEO1.config(serial_port, UBX_protocol::GLL, UBX_protocol::GP_Length); // disable GPGLL
  NEO1.config(serial_port, UBX_protocol::GSA, UBX_protocol::GP_Length); // disable GSA
  NEO1.config(serial_port, UBX_protocol::GSV, UBX_protocol::GP_Length); // disable GPGSV
  NEO1.config(serial_port, UBX_protocol::RMC, UBX_protocol::GP_Length); // disable RMC
  NEO1.config(serial_port, UBX_protocol::VTG, UBX_protocol::GP_Length); // disable VTG

/* START LOGGING*/
  startLogging(); //Der er 2 måder logger alt eller logger kun når fil bliver kaldt. Det nemmeste er nok alt for at slippe for en klasse hovedpine for alt data.

/* STARTING LOOP*/
  while(1)
    {
      
      NEO1.readGPS(serial_port, GPS_data, d1, d2); // reads NMEA message
      
      Long = NEO1.getLongitude();    // returns longitude
      Lat = NEO1.getLatitude();      // returns latitude
      NS = NEO1.getSouthNorth();     // returns either a north pole or south pole
      EW = NEO1.getEastWest();       // returns either a East pole or West pole

      NEO1.convertdata(Long, Lat, NS, EW); //converts to decimal degrees format

      usleep(1000000); // delay 1 second
    }

  return 0;

}