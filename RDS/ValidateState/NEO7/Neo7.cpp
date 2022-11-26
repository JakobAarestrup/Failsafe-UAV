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

    return fd;
}

void GPS::configAll()
{
    /* OPEN UART */
    serialPort_ = openUART(serialPort_);

    /* CONFIGURATION */

    /*NMEA Config*/
    write(serialPort_, UBX_protocol::NMEA_CFG, UBX_protocol::NMEA_CFG_Length); // disable SBAS QZSS GLONASS BeiDou Galileo

    /*Update Rate*/
    write(serialPort_, UBX_protocol::RATE, UBX_protocol::RATE_Length); // Measurement frequency: 10 hz, navigation frequency 10 hz

    /*NMEA messages*/
    write(serialPort_, UBX_protocol::GLL, UBX_protocol::GP_Length); // disable GPGLL
    write(serialPort_, UBX_protocol::GSA, UBX_protocol::GP_Length); // disable GSA
    write(serialPort_, UBX_protocol::GSV, UBX_protocol::GP_Length); // disable GPGSV
    write(serialPort_, UBX_protocol::RMC, UBX_protocol::GP_Length); // disable RMC
    write(serialPort_, UBX_protocol::VTG, UBX_protocol::GP_Length); // disable VTG
    printf("Configuration is done! \n");
}

void GPS::readGPS() // reads GPS serial data
{

    /*VARIABLES*/
    char buff[100], GGA_Check[3];
    unsigned char GGA_Flag = 0;
    unsigned char GGA_Index = 0;
    unsigned char GGA_Received = 0;
    char GPS_Data = 0;
    char sd;
    int end = 0;

    char *start_ptr, *end_ptr, *jump_ptr, *gps;

    while (end < 1)
    {

        // receive character serially

        if ((GPS_Data = serialGetchar(serialPort_)) == '$') // check for start of NMEA message
        {
            GGA_Flag = 0;
            GGA_Index = 0;
        }

        else if (GGA_Flag == 1)
        {
            buff[GGA_Index++] = GPS_Data;

            if (GPS_Data == '\r')
            {
                GGA_Received = 1;
            }
        }

        else if (GGA_Check[0] == 'G' && GGA_Check[1] == 'G' && GGA_Check[2] == 'A')
        {
            GGA_Flag = 1;
            GGA_Check[0] = 0;
            GGA_Check[0] = 0;
            GGA_Check[0] = 0;
        }

        else if (GGA_Received == 1)
        {
            // printf("GPGGA:%s\n", buff);
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

            printf("latitude: %f %s longitude: %f %s Satellites: %d\n\n", latitude_, NS_, longitude_, EW_, SV_);

            end = 1;
        }
        else
        {
            GGA_Check[0] = GGA_Check[1];
            GGA_Check[1] = GGA_Check[2];
            GGA_Check[2] = GPS_Data;
        }

        /*      else
             {
                 printf("No data available from GPS\n");
                 serialPort_ = openUART(serialPort_);
                 end = 1;
             } */

                printf("%c|", GPS_Data);
    }
}

void GPS::convertData() // converts GPS serial data to decimal degrees
{
    double lat_Deg = int(latitude_) / 100;  // (d)dd(deg)
    double lon_Deg = int(longitude_) / 100; // (d)dd(deg)

    double lat_Sec = (latitude_ - lat_Deg * 100) / 60;  // mm.mmmm(minutes) / 60 = seconds
    double lon_Sec = (longitude_ - lon_Deg * 100) / 60; // mm.mmmm(minutes) / 60 = seconds

    char NS[1];
    char EW[1];

    getNorthSouth(NS);
    getNorthSouth(EW);

    if (strcmp(NS_, "") == 0 | strcmp(EW_, "") == 0) // is 1 of the arrays empty?
    {
        std::cout << "NS or EW returned N/A. Skipping conversion..." << std::endl;
    }

    else
    {
        if (strcmp(NS_, "S") == 0 & strcmp(EW_, "E") == 0) // handles negative
        {
            latitude_ = (lat_Deg + lat_Sec) * -1;
            longitude_ = lon_Deg + lon_Sec;
        }
        else if (strcmp(NS_, "N") == 0 & strcmp(EW_, "W") == 0)
        {

            latitude_ = lat_Deg + (lat_Sec);
            longitude_ = lon_Deg + (lon_Sec) * -1;
        }
        else if (strcmp(NS_, "S") == 0 & strcmp(EW_, "W") == 0)
        {
            latitude_ = lat_Deg + (lat_Sec) * -1;
            longitude_ = lon_Deg + (lon_Sec) * -1;
        }
        else
        {
            latitude_ = lat_Deg + lat_Sec;
            longitude_ = lon_Deg + lon_Sec;
        }

        // std::cout << "" << latitude_ << "," << NS_[1] << " " << longitude_ << "," << EW_[1] << " Satellites:" << SV_ << std::endl;
    }
}

void GPS::startLogging()
{
    freopen("RDSLog.txt", "w", stdout); // https://stackoverflow.com/questions/7400418/writing-a-log-file-in-c-c
                                        // cout << "" << longitude_ << "," << NS_ << " " << latitude_ << "," << EW_ <<  "  Satellites: " << SV_ << endl;
}

/* GET FUNCTIONS */

int GPS::getSV() const // returns amount of satellites
{
    return SV_;
}

double GPS::getLongitude() const // returns longitude
{
    return longitude_;
}

double GPS::getLatitude() const // returns latitude
{
    return latitude_;
}

void GPS::getNorthSouth(char NS[]) // returns either a East pole or West pole
{
    int strLength = strlen(NS_); // finds length of the array
    for (int i = 0; i < strLength; i++)
    {
        NS[i] = NS_[strLength - 1 - i]; // copies UserInput in reverse to TempInput
    }
    NS[strLength] = '\0'; // adds NULL character at end
}

void GPS::getEastWest(char EW[]) // returns either a East pole or West pole
{
    int strLength = strlen(EW_); // finds length of the array
    for (int i = 0; i < strLength; i++)
    {
        EW[i] = EW_[strLength - 1 - i]; // copies UserInput in reverse to TempInput
    }
    EW[strLength] = '\0'; // adds NULL character at end
}
