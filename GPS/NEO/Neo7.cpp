#include "Neo7.hpp"
#include "unistd.h"

GPS::GPS() // default constructor
{
    printf("Constructor called \n");
} 

GPS::~GPS() // destructor
{
    //delete[] longitude_, latitude_; // delete
    printf("Destructor called\n");
}

int GPS::openUART(int fd) // open UART serial port
{

    if ((fd = serialOpen("/dev/ttyS0", 9600)) < 0) // open serial port with set baudrate
        {
            fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)); // error handling
            return 1;
        }


    if (wiringPiSetup() == -1)	// initializes wiringPi setup
        {
            fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)); // error handling
            return 1;
        }
        
    return fd;
}

void GPS::config(int fd, const char* message, size_t length) // configuration of the GPS
{
    write(fd, message, length); // sends CFG string to serial 
}

void GPS::readGPS(int fd, char sensor_Data, char* d1 , char* d2) // reads GPS serial data
{
    /*VARIABLES*/
    char buff[100], GGA_code[3];
    unsigned char IsitGGAstring=0;
    unsigned char GGA_index=0;
    unsigned char is_GGA_received_completely = 0;
    char* NMEA[16]; // array for ASCII tokens

    int count = 0; // counter for for loop
    int i = 0; // increment for for loop
    while(1)
    {

        if(serialDataAvail(fd))		/* check for any data available on serial port */
        { 
            sensor_Data = serialGetchar(fd);		/* receive character serially */	
            printf("%c",sensor_Data);
            
            if(sensor_Data == '$') //check for start of NMEA message
                {
                    IsitGGAstring = 0;
                    GGA_index = 0;
                }

            else if(IsitGGAstring ==1)
                {
                    buff[GGA_index++] = sensor_Data;

                    if(sensor_Data == '\r')
                        {
                            is_GGA_received_completely = 1;
                        }
                }

            else if(GGA_code[0] =='G' && GGA_code[1] =='G' && GGA_code[2] =='A')
                {
                IsitGGAstring = 1;
                GGA_code[0]= 0; 
                GGA_code[0]= 0;
                GGA_code[0]= 0;		
                }

            else
                {
                GGA_code[0] = GGA_code[1];
                GGA_code[1] = GGA_code[2];
                GGA_code[2] = sensor_Data;
                }
        }

        if(is_GGA_received_completely==1)
        {
            printf("\nGGA: %s\n",buff); // kan udkommenteres
            char* b1 = strtok(buff,","); // first token
            printf("b1 = %s\n",b1); //

            for(i = 0 ; i < 15 ; i++)
            {

                if (b1 == NULL)
                {
                NMEA[count] = d1; // 0
                printf("HELLO IF\n");
                }
                else if (b1 == NULL && count == 3 || 5)
                {
                printf("HELLO ELSE IF\n");
                NMEA[count] = d2; // N/A
                }
                else
                {
                NMEA[count] = b1; 
                printf("HELLO\n");
                }

                count++; // increment
                b1 = strtok(NULL, ",");
                printf("b1 = %s\n",b1); 

            }
            //printf("NMEA: %s\n",NMEA);
            NS_ = NMEA[3]; 
            EW_ = NMEA[5]; 

            printf("%c, %c\n",NS_,EW_);

            //conversion
            latitude_   = atof(NMEA[2]);
            longitude_  = atof(NMEA[4]);
            SV_ = atoi(NMEA[7]);
            printf("Lat: %f Long: %f SV: %d\n",latitude_,longitude_,SV_);
        
            i = 0; // reset i variable
            count = 0; // reset count variable
            is_GGA_received_completely = 0; // reset GGA receive flag variable
            break;
        }
    }    
}

void GPS::convertData(float lon_Data, float lat_Data, char* NS, char* EW) // converts GPS serial data to degrees
{
float lat_Deg = int(lat_Data)/100; 
float lon_Deg = int(lon_Data)/100;

float lat_Sec = (lat_Data-lat_Deg*100);
float lon_Sec = (lon_Data-lon_Deg*100);

//(cout << lon_Deg <<" , " << lon_Sec << endl; // (d)dd(deg) mm.mmmm(minutes)
//cout << lat_Deg <<" , " << lat_Sec << endl; // (d)dd(deg) mm.mmmm(minutes)

if (NS=="S" & EW == "E" ) // handles negative
{
    latitude_  = (lat_Deg  + (lat_Sec/60))*-1;
    longitude_ = lon_Deg  + (lon_Sec/60);
}
else if (NS=="N" & EW == "W")
{
    latitude_  = lat_Deg  + (lat_Sec/60);
    longitude_ = (lon_Deg  + (lon_Sec/60))*-1;
}
else if(NS == "S" & EW == "W")
{
    latitude_  = (lat_Deg  + (lat_Sec/60))*-1;
    longitude_ = (lon_Deg  + (lon_Sec/60))*-1;
}
else
{
    latitude_  = lat_Deg  + (lat_Sec/60);
    longitude_ = lon_Deg  + (lon_Sec/60);
}

//cout << latitude_ << ", " << longitude_ << endl; // decimal degrees
std::cout << "" << longitude_ << "," << NS_ << " " << latitude_ << "," << EW_ << " Satellites:" << SV_ << std::endl;

}

void GPS::startLogging()
{
   freopen("RDSLog.txt", "w", stdout); // https://stackoverflow.com/questions/7400418/writing-a-log-file-in-c-c
   //cout << "" << longitude_ << "," << NS_ << " " << latitude_ << "," << EW_ <<  "  Satellites: " << SV_ << endl;
}


/* GET FUNCTIONS */

int GPS::getSV() const // returns amount of satellites
{
    return SV_; 
}

float GPS::getLongitude() const // returns longitude
{
    return longitude_;
}

float GPS::getLatitude() const // returns latitude
{
    return latitude_;
}

char* GPS::getEastWest() const  // returns either a East pole or West pole
{
    return EW_;
}

char* GPS::getNorthSouth() const  // returns either a East pole or West pole
{
    return NS_;
}