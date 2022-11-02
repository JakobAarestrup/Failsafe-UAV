#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include <wiringPi.h>
#include <wiringSerial.h>


//#pragma once // skal denne bruges?

class GPS
{
    public:
        GPS();  // default constructor
        ~GPS(); // destructor

        int openUART(int fd); // open UART serial port
        void config(int fd, const char* message, size_t length);              // configuration of the GPS
        void convertData(double lon_Data, double lat_Data, char* NS, char* EW);   // converts GPS data
        void readGPS(int fd, char sensor_Data, char* d1 , char* d2); // reads GPS serial data
        void startLogging(); // sends private data to a log file 

        int   getSV() const;         // returns amount of satellites
        double getLongitude() const;  // returns longitude
        double getLatitude()  const;  // returns latitude
        char getNorthSouth() const; // returns either a north pole or south pole
        char getEastWest() const;   // returns either a East pole or West pole

    private: // Coordinates from the GPS

        int SV_; // amount of satellites
        double longitude_; 
        double latitude_;
        char EW_[1];
        char NS_[1];
};