#pragma once
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <fcntl.h>
#include <termios.h>

#include "UBX_Protocol_Constants.hpp"
#include <wiringPi.h>
#include <wiringSerial.h>

struct GPSPosition
{
    double longitude, latitude;
    int SV;
};

class GPS
{
public:
    GPS();                // default constructor
    ~GPS();               // destructor
    int openUART(int fd); // open UART serial port
    int configAll();
    void readGPS();                     // reads GPS serial data
    void convertData();                 // converts GPS data
    GPSPosition getGPSPosition() const; // gets GPS position

    int getSV() const;             // returns amount of satellites
    double getLongitude() const;   // returns longitude
    double getLatitude() const;    // returns latitude
    void getNorthSouth(char NS[]); // returns either a north pole or south pole
    void getEastWest(char EW[]);   // returns either a East pole or West pole

private:     // Coordinates from the GPS
    int SV_; // amount of satellites
    double longitude_;
    double latitude_;
    char EW_[10];
    char NS_[10];
    int serialPort_;
    char GPS_Data_;
};