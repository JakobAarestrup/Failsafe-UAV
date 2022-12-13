#include "GPS.hpp"
#include "unistd.h"
#include <fcntl.h>
#include <termios.h>

/**
 * @brief Construct a new GPS::GPS object
 *
 */
GPS::GPS()
{
    SV_ = 0;
    EW_ = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
    NS_ = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
    longitude_ = 0;
    latitude_ = 0;
    serialPort_ = 0;
    GPS_data_ = '0';
}

/**
 * @brief Destroy the GPS::GPS object
 *
 */
GPS::~GPS() // destructor
{
}

/**
 * @brief Funtion opens the UART serial port to the GPS
 *
 * @param fd is the used file descriptor
 * @return int fd
 */
int GPS::openUART(int fd) // open UART serial port
{
    if ((fd = serialOpen("/dev/ttyS0", 9600)) < 0) // open serial port with set baudrate
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno)); // error handling

        return 1;
    }
    return fd;
}

/**
 * @brief Function configures the GPS to work with 4800 baudrate and return the desired messages
 *
 * @return int serialPort_
 */
int GPS::configAll()
{
    /*OPEN UART*/
    if ((serialPort_ = serialOpen("/dev/ttyS0", 9600)) < 0) // open serial port with set baudrate
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno)); // error handling

        return 1;
    }

    if (wiringPiSetup() == -1) // initializes wiringPi setup
    {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno)); // error handling
        return 1;
    }

    /* CONFIGURATION */

    /*NMEA Config*/
    write(serialPort_, UBX_protocol::NMEA_CFG, UBX_protocol::NMEA_CFG_Length); // disable SBAS QZSS GLONASS BeiDou Galileo

    /*Update Rate*/
    write(serialPort_, UBX_protocol::RATE, UBX_protocol::RATE_Length); // Measurement frequency: 5 hz, navigation frequency 10 hz

    /*NMEA messages*/
    write(serialPort_, UBX_protocol::GLL, UBX_protocol::GP_Length); // disable GPGLL
    write(serialPort_, UBX_protocol::GSA, UBX_protocol::GP_Length); // disable GSA
    write(serialPort_, UBX_protocol::GSV, UBX_protocol::GP_Length); // disable GPGSV
    write(serialPort_, UBX_protocol::RMC, UBX_protocol::GP_Length); // disable RMC
    write(serialPort_, UBX_protocol::VTG, UBX_protocol::GP_Length); // disable VTG

    printf("Configuration is done! \n");

    serialClose(serialPort_);

    return serialPort_;
}

/**
 * @brief Function reads serial data from the GPS
 *
 */
void GPS::readGPS() // reads GPS serial data
{

    /*VARIABLES*/
    char buff[100], GGA_Check[3] = {'0', '0', '0'};
    unsigned char GGA_Flag = 0;
    unsigned char GGA_Index = 0;
    unsigned char GGA_Received = 0;
    char *start_ptr, *end_ptr, *jump_ptr, *gps;
    // int i = 0;
    //  configAll();

    /* OPEN UART */
    serialPort_ = openUART(serialPort_);
    for (int i = 0; i < 200; i++)
    {

        GPS_data_ = serialGetchar(serialPort_); /* receive character serially */
        // printf("%c", GPS_data_);
        //     read(serialPort_, &GPS_data_, 1);
        if (GPS_data_ == '$') // check for start of NMEA message
        {
            GGA_Flag = 0;
            GGA_Index = 0;
        }

        else if (GGA_Flag == 1)
        {
            buff[GGA_Index++] = GPS_data_;

            if (GPS_data_ == '\r')
            {
                GGA_Received = 1;
            }
        }
        else if (GGA_Check[0] == 'G' && GGA_Check[1] == 'G' && GGA_Check[2] == 'A')
        {
            GGA_Flag = 1;
            GGA_Check[0] = 0;
            GGA_Check[1] = 0;
            GGA_Check[2] = 0;
        }
        else
        {
            GGA_Check[0] = GGA_Check[1];
            GGA_Check[1] = GGA_Check[2];
            GGA_Check[2] = GPS_data_;
        }

        if (GGA_Received == 1)
        {
            gps = buff;
            start_ptr = strchr(gps, ',');       // find start of latitude field
            end_ptr = strchr(++start_ptr, ','); // find end of field...
            latitude_ = atof(start_ptr);        // Convert char to float & store in variable

            start_ptr = strchr(start_ptr, ','); // find start of pole NS field
            end_ptr = strchr(++start_ptr, ','); // find end of field... ¨
            jump_ptr = end_ptr;
            *end_ptr = '\0';
            strcpy(NS_, start_ptr);

            // printf(" lat: %f DDDDDDDDDD:%s\n",latitude,NS_);

            start_ptr = jump_ptr;               // find start of longitude field
            end_ptr = strchr(++start_ptr, ','); // find end of field...
            jump_ptr = end_ptr;
            *end_ptr = '\0'; // and zero terminate
            longitude_ = atof(start_ptr);

            // printf(" lat: %f D:%s long: %f\n",latitude,NS_,longitude);

            start_ptr = jump_ptr;               // find start of pole EW field
            end_ptr = strchr(++start_ptr, ','); // find end of field...
            *end_ptr = '\0';                    // and zero terminate
            strcpy(EW_, start_ptr);

            start_ptr = strchr(++end_ptr, ','); // find start of satellite field
            end_ptr = strchr(++start_ptr, ','); // find end of field...
            *end_ptr = '\0';                    // and zero terminate
            SV_ = atoi(start_ptr);              // Convert char to int & store in variable

            // printf("latitude: %f %s longitude: %f %s Satellites: %d\n\n", latitude_, NS_, longitude_, EW_, SV_);
            //    end = 1;
            i = 200;
        }
    }
    serialClose(serialPort_);
}

/**
 * @brief Function converts the GPS serial data to decimal degrees
 *
 */
void GPS::convertData()
{
    char NS[1];
    char EW[1];

    double lat_Deg = int(latitude_) / 100;  // (d)dd(deg)
    double lon_Deg = int(longitude_) / 100; // (d)dd(deg)

    double lat_Sec = (latitude_ - lat_Deg * 100) / 60;  // mm.mmmm(minutes) / 60 = seconds
    double lon_Sec = (longitude_ - lon_Deg * 100) / 60; // mm.mmmm(minutes) / 60 = seconds

    getNorthSouth(NS);
    getEastWest(EW);

    if ((strcmp(NS, "") == 0) | (strcmp(EW, "") == 0)) // is 1 of the arrays empty?
    {
        std::cout << "NS or EW returned N/A. Skipping conversion..." << std::endl;
    }

    else
    {
        if ((strcmp(NS, "S") == 0) & (strcmp(EW, "E") == 0)) // handles negative
        {
            latitude_ = (lat_Deg + lat_Sec) * -1;
            longitude_ = lon_Deg + lon_Sec;
        }
        else if ((strcmp(NS, "N") == 0) & (strcmp(EW, "W") == 0))
        {

            latitude_ = lat_Deg + lat_Sec;
            longitude_ = (lon_Deg + lon_Sec) * -1;
        }
        else if ((strcmp(NS, "S") == 0) & (strcmp(EW, "W") == 0))
        {
            latitude_ = (lat_Deg + lat_Sec) * -1;
            longitude_ = (lon_Deg + lon_Sec) * -1;
        }
        else
        {
            latitude_ = lat_Deg + lat_Sec;
            longitude_ = lon_Deg + lon_Sec;
        }
    }
}

/**
 * @brief Function to get the GPS position
 *
 * @return GPSPosition
 */
GPSPosition GPS::getGPSPosition()
{
    char NS[1];
    char EW[1];

    getNorthSouth(NS);
    getEastWest(EW);

    return {longitude_, latitude_, SV_, NS[0], EW[0]};
}

/**
 * @brief Function to get the amount of connected satellites
 *
 * @return int SV_
 */
int GPS::getSV() const
{
    return SV_;
}

/**
 * @brief Function to get the longitude from the GPS
 *
 * @return double longitude_
 */
double GPS::getLongitude() const
{
    return longitude_;
}

/**
 * @brief Function to get the latitude from the GPS
 *
 * @return double latitude_
 */
double GPS::getLatitude() const
{
    return latitude_;
}

/**
 * @brief Function fills the North South array with the input
 *
 * @param NS is the North South array input
 */
void GPS::getNorthSouth(char NS[])
{
    int strLength = strlen(NS_); // finds length of the array
    for (int i = 0; i < strLength; i++)
    {
        NS[i] = NS_[i];
    }
    NS[strLength] = '\0'; // adds NULL character at end
}

/**
 * @brief Function fills the East West array with the input
 *
 * @param NS is the East West array input
 */
void GPS::getEastWest(char EW[])
{
    int strLength = strlen(EW_); // finds length of the array
    for (int i = 0; i < strLength; i++)
    {
        EW[i] = EW_[i];
    }
    EW[strLength] = '\0'; // adds NULL character at end
}
