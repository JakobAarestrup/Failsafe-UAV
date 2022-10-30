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
        void convertData(float lon_Data, float lat_Data);            // converts GPS data
        void readGPS(int fd, char sensor_Data, char* d1 , char* d2); // reads GPS serial data

        float getLongitude() const;    // returns longitude
        float getLatitude()  const;    // returns latitude
        int   getSV() const;           // returns amount of satellites
    
    private: // Coordinates from the GPS

        int SV_; // amount of satellites
        float longitude_; 
        float latitude_;
        char* WE_;
        char* NS_;
}