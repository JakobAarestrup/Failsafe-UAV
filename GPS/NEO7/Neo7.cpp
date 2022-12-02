#include "Neo7.hpp"
#include "unistd.h"

GPS::GPS() // default constructor
{
    // printf("Constructor called \n");
}

GPS::~GPS() // destructor
{
    // delete[] longitude_, latitude_; // delete
    // printf("Destructor called \n");
}

int GPS::openUART(int fd) // open UART serial port
{

    if ((fd = serialOpen("/dev/ttySOFT0", 4800)) < 0) // open serial port with set baudrate
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno)); // error handling

        return 1;
    }

    if (wiringPiSetup() == -1) // initializes wiringPi setup
    {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno)); // error handling
        return 1;
    }

    return fd;
}

void GPS::configAll(int fd)
{
    /*OPEN UART*/
    if ((fd = serialOpen("/dev/ttyS0", 9600)) < 0) // open serial port with set baudrate
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
    write(serialPort_, UBX_protocol::RATE, UBX_protocol::RATE_Length); // Measurement frequency: 10 hz, navigation frequency 10 hz

    /*BAUDRATE */
    write(serialPort_, UBX_protocol::BAUDRATE, UBX_protocol::BAUD)

    /*NMEA messages*/
    write(serialPort_, UBX_protocol::GLL, UBX_protocol::GP_Length); // disable GPGLL
    write(serialPort_, UBX_protocol::GSA, UBX_protocol::GP_Length); // disable GSA
    write(serialPort_, UBX_protocol::GSV, UBX_protocol::GP_Length); // disable GPGSV
    write(serialPort_, UBX_protocol::RMC, UBX_protocol::GP_Length); // disable RMC
    write(serialPort_, UBX_protocol::VTG, UBX_protocol::GP_Length); // disable VTG
    printf("Configuration is done! \n");
    serialClose(serialPort_);
    return fd;
}

void GPS::readGPS() // reads GPS serial data
{

    /*VARIABLES*/
    char buff[100], GGA_Check[3];
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

        GPS_Data_ = serialGetchar(serialPort_); /* receive character serially */
        // printf("%c", GPS_Data_);
        //  read(serialPort_, &GPS_Data, 1);
        if (GPS_Data_ == '$') // check for start of NMEA message
        {
            GGA_Flag = 0;
            GGA_Index = 0;
        }

        else if (GGA_Flag == 1)
        {
            buff[GGA_Index++] = GPS_Data_;

            if (GPS_Data_ == '\r')
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
            GGA_Check[2] = GPS_Data_;
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
            //  end = 1;
            i = 200;
        }
    }
    serialClose(serialPort_);
}