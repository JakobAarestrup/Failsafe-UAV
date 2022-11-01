#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <cstddef>

/*Ownlibraries*/
#include "UBX_Protocol_Constants.hpp"
#include "Neo7.hpp"

int main()
{
/* VARIABLES */
  char GPS_data;
  char def_1[] = "0";
  char def_2[] = "N/A";

  char* d1 = def_1;
  char* d2 = def_2;

  float Long = 0.0, Lat = 0.0;

  int serial_port = 0;
  int NS = 0, EW = 0;
  
  
/* OPEN UART */
  serial_port = openUart(serial_port); // opens serial port

/* CONFIGURATION */
  
  /*NMEA Config*/
  config(serial_port, UBX_protocol::NMEA_CFG, UBX_protocol::NMEA_CFG_Length); // disable SBAS QZSS GLONASS BeiDou Galileo
  
  /*Update Rate*/
  config(serial_port, UBX_protocol::RATE, UBX_protocol::RATE_Length); // Measurement frequency: 10 hz, navigation frequency 10 hz
 
  /*NMEA messages*/
  config(serial_port, UBX_protocol::GLL, UBX_protocol::GP_Length); // disable GPGLL
  config(serial_port, UBX_protocol::GSA, UBX_protocol::GP_Length); // disable GSA
  config(serial_port, UBX_protocol::GSV, UBX_protocol::GP_Length); // disable GPGSV
  config(serial_port, UBX_protocol::RMC, UBX_protocol::GP_Length); // disable RMC
  config(serial_port, UBX_protocol::VTG, UBX_protocol::GP_Length); // disable VTG

/* START LOGGING*/
  startLogging(); //Der er 2 måder logger alt eller logger kun når fil bliver kaldt. Det nemmeste er nok alt for at slippe for en klasse hovedpine for alt data.

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