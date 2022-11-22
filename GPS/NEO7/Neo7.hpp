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
        void configAll();
        void readGPS(); // reads GPS serial data
        void convertData();   // converts GPS data
        void startLogging(); // sends private data to a log file 

        int getSV() const;             // returns amount of satellites
        double getLongitude() const;   // returns longitude
        double getLatitude()  const;   // returns latitude
        void getNorthSouth(char NS[]); // returns either a north pole or south pole
        void getEastWest(char EW[]);   // returns either a East pole or West pole

    private: // Coordinates from the GPS

        int SV_; // amount of satellites
        double longitude_; 
        double latitude_;
        char EW_[10];
        char NS_[10];
        int serialPort_;
        char GPS_Data_
};