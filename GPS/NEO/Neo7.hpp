#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include <wiringPi.h>
#include <wiringSerial.h>


#pragma once // skal denne bruges?

class GPS
{
    public:
        GPS();  // default constructor
        ~GPS(); // destructor

        void openUART(int fd); // open UART serial port
        void config(int fd, char* message, size_t length)            // configuration of the GPS
        void convertData(float lon_Data, float lat_Data);            // converts GPS data
        void readGPS(int fd, char sensor_Data, char* d1 , char* d2); // reads GPS serial data
        void startLogging(); // sends private data to a log file 

        int   getSV() const;         // returns amount of satellites
        float getLongitude() const;  // returns longitude
        float getLatitude()  const;  // returns latitude
        char  getNorthSouth() const; // returns either a north pole or south pole
        char  getEastWest() const;   // returns either a East pole or West pole

    private: // Coordinates from the GPS

        int SV_; // amount of satellites
        float longitude_; 
        float latitude_;
        char* EW_;
        char* NS_;
}