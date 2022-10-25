#include "Neo7.hpp"

GPS::GPS() // default constructor
{
    printf("Constructor called \n");
} 

GPS::~GPS(); // destructor
{
    delete[] longitude_, latitude_; // delete
    printf("Destructor called\n");
}

void GPS::openUART(int fd) // open UART serial port
{

    if ((fd = serialOpen("/dev/ttyS0", 9600)) < 0) // open serial port with set baudrate
        {
            fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)); // error handling
            return 1 ;
        }
    return fd;

    if (wiringPiSetup() == -1)	// initializes wiringPi setup
        {
            fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)); // error handling
            return 1 ;
        }

}

void GPS::config(int fd, char message) // configuration of the GPS
{
    serialPuts(fd, message); // sends CFG string to serial 
}

void GPS::readGPS(int fd, char sensor_Data) // reads GPS serial data
{
    if(serialDataAvail(fd))
	    { 
            int SV;  // number of satellites
            float lon; // longitude
            float lat; // latitude
            char data_String[6]; // array for picking up NMEA main talker
            
            sensor_Data = serialGetchar (fd);		// receive character serially

            /* if(sensor_Data == $GPGGA)
                {
                    SV_ =
                    longitude_ = 
                    latitude_  =
                } */
            printf ("%c", sensor_Data););
            fflush (stdout);
            serialPutchar(sensor_Data, fd);		// transmit character serially on port
	    }
}

void GPS::convertData(float lon_data, float lat_data) // converts GPS serial data to degrees
{
    int lon_1 = int(lon_data);
    int lat_1 = int(lat_data);
    float lon_2 = 
    float lat_2 =
}

int GPS::getSV() const
{
    return SV_; // returns amount of satellites
}

int GPS::getLongitude() const // returns latitude
{
    return longitude_;
}

int GPS::getLatitude() const // returns latitude
{
    return latitude_;
}