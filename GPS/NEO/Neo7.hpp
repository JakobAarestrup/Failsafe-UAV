#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>


#pragma once // skal denne bruges?

class GPS
{
    public:
        GPS();  // default constructor
        ~GPS(); // destructor

        void openUART(int fd);              // open UART serial port
        void config(int fd, char message);  // configuration of the GPS
        void readGPS();                     // reads GPS serial data
        void convertData();                 // converts GPS data

        float getLongitude() const;    // returns longitude
        float getLatitude()  const;    // returns latitude
        int   getSV() const;           // returns amount of satellites
    
    private: // Cordinates from the GPS

        int SV_; // amount of satellites
        float longitude_; 
        float latitude_;
}