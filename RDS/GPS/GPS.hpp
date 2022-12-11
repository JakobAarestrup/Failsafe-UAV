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

/**
 * @brief Struct that contains the GPS data
 *
 */
struct GPSPosition
{
    double longitude, latitude;
    int SV;
    char NS[1];
    char EW[1];
};

class GPS
{
public:
    GPS();                // default constructor
    ~GPS();               // destructor
    int openUART(int fd); // open UART serial port
    int configAll();
    void readGPS();               // reads GPS serial data
    void convertData();           // converts GPS data
    GPSPosition getGPSPosition(); // gets GPS position

    int getSV() const;             // returns amount of satellites
    double getLongitude() const;   // returns longitude
    double getLatitude() const;    // returns latitude
    void getNorthSouth(char NS[]); // returns either a north pole or south pole
    void getEastWest(char EW[]);   // returns either a East pole or West pole

private:
    /// @brief Amount of satellites
    int SV_;
    /// @brief East West array
    char EW_[10];
    /// @brief North South array
    char NS_[10];

    /// @brief Longitude
    double longitude_;
    /// @brief Latitude
    double latitude_;
    /// @brief Serial port file descriptor

    int serialPort_;
    /// @brief Data from the GPS
};