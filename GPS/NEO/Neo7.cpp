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
            char data_String[6]; // array for picking up NMEA main talker
            
            sensor_Data = serialGetchar (fd);		// receive character serially

            /* if(sensor_Data == $GPGGA)
                {
                    SV_ =
                    longitude_ = 
                    latitude_  =
                } */
            
            printf ("%c", sensor_Data);
            fflush (stdout);
            serialPutchar(sensor_Data, fd);		// transmit character serially on port
	    }
}

void GPS::convertData(float lon_Data, float lat_Data) // converts GPS serial data to degrees
{
float lon_Deg = int(lon_Data)/100;
float lat_Deg = int(lat_Data)/100; 

float lon_Sec = (amount-lon_Deg*100);
float lat_Sec = (amount2-lat_Deg*100);

cout << lon_Deg <<" , " << lon_Sec << endl; // (d)dd(deg) mm.mmmm(seconds)
cout << lat_Deg <<" , " << lat_Sec << endl; // (d)dd(deg) mm.mmmm(seconds)

if (S==1 & W == 0 ) // handles negative
{
    latitude  = (lat_Deg  + (lat_Sec/60))*-1;
    longitude = lon_Deg  + (lon_Sec/60);
}
else if (S==0 & W == 1)
{
    latitude_  = lat_Deg  + (lat_Sec/60);
    longitude_ = (lon_Deg  + (lon_Sec/60))*-1;
}
else if(S == 1 & W == 1)
{
    latitude_  = (lat_Deg  + (lat_Sec/60))*-1;
    longitude_ = (lon_Deg  + (lon_Sec/60))*-1;
}
else
{
    latitude_  = lat_Deg  + (lat_Sec/60);
    longitude_ = lon_Deg  + (lon_Sec/60);
}

cout << latitude << ", " << longitude << endl; // decimal degrees

return 0;

}

int GPS::getSV() const // returns amount of satellites
{
    return SV_; 
}

int GPS::getLongitude() const // returns longitude
{
    return longitude_;
}

int GPS::getLatitude() const // returns latitude
{
    return latitude_;
}