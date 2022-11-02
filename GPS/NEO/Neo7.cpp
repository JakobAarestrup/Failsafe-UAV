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
    char *start_ptr, *end_ptr, *start_ptr_origin, *jump_ptr;
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
        printf("GGA:%s\n",buff);
        char *gps = buff; 
        start_ptr = strchr(gps, ','); // find start of latitude field
        end_ptr = strchr(++start_ptr, ',');  // find end of field... 
        latitude_ = atof(start_ptr);   // Convert char to float & store in variable
        
        start_ptr = strchr(start_ptr, ',');  // find start of pole NS field
        end_ptr = strchr(++start_ptr, ','); // find end of field... ¨
        jump_ptr = end_ptr;
        *end_ptr = '\0';
        strcpy(NS_, start_ptr);

        //printf(" lat: %f DDDDDDDDDD:%s\n",latitude,NS_);
        
        start_ptr = jump_ptr; // find start of longitude field
        end_ptr = strchr(++start_ptr, ','); // find end of field... 
        jump_ptr = end_ptr; 
        *end_ptr = '\0';  // and zero terminate
        longitude_ = atof(start_ptr); 

        //printf(" lat: %f D:%s long: %f\n",latitude,NS_,longitude);

        start_ptr = jump_ptr; // find start of pole EW field
        end_ptr = strchr(++start_ptr, ','); // find end of field...
        *end_ptr = '\0';  // and zero terminate 
        strcpy(EW_, start_ptr);
        
        start_ptr = strchr(++end_ptr, ','); // find start of satellite field
        end_ptr = strchr(++start_ptr, ','); // find end of field... 
        *end_ptr = '\0';  // and zero terminate
        SV_ = atoi(start_ptr); // Convert char to int & store in variable
        
        printf("latitude: %f ,%s longitude: %f ,%s Satelites: %d\n\n", latitude_, NS_, longitude_, EW_, SV_);
        break;
        }
    }            
}

void GPS::convertData(double lon_Data, double lat_Data, char NS[], char EW[]) // converts GPS serial data to degrees
{
double lat_Deg = int(lat_Data)/100; 
double lon_Deg = int(lon_Data)/100;

double lat_Sec = (lat_Data-lat_Deg*100);
double lon_Sec = (lon_Data-lon_Deg*100);
//strcpy(EW_,"W");

//(cout << lon_Deg <<" , " << lon_Sec << endl; // (d)dd(deg) mm.mmmm(minutes)
//cout << lat_Deg <<" , " << lat_Sec << endl; // (d)dd(deg) mm.mmmm(minutes)
printf("NS:%s\n",NS);
printf("EW:%s\n",EW);
if(strcmp(NS,"\0") == 1 | strcmp(NS,"\0") == 1) 
{
std::cout << "NS returned N/A. Skipping conversion..." << std::endl;
//std::cout << "" << latitude_ << "," << NS_[1] << " " << longitude_ << "," << EW_[1] << " Satellites:" << SV_ << std::endl;
}

else if(strcmp(EW,"E") == 1 | strcmp(NS,"W") == 1)
{
std::cout << "EW retuned N/A. Skipping conversion..." << std::endl;    
}

else
{
    if (strcmp(NS,"S") == 0 & strcmp(EW, "E") == 0 ) // handles negative
    {
        latitude_  = (lat_Deg  + (lat_Sec/60))*-1;
        longitude_ = lon_Deg  + (lon_Sec/60);
    }
    else if (strcmp(NS,"N") == 0 & strcmp(EW, "W") == 0)
    {

        latitude_  = lat_Deg  + (lat_Sec/60);
        longitude_ = (lon_Deg  + (lon_Sec/60))*-1;
        printf("HELLO\n");
    }
    else if (strcmp(NS,"S") == 0 & strcmp(EW, "W") == 0)
    {
        latitude_  = (lat_Deg  + (lat_Sec/60))*-1;
        longitude_ = (lon_Deg  + (lon_Sec/60))*-1;
    }
    else
    {
        latitude_  = lat_Deg  + (lat_Sec/60);
        longitude_ = lon_Deg  + (lon_Sec/60);
    }
std::cout << "" << latitude_ << "," << NS_[1] << " " << longitude_ << "," << EW_[1] << " Satellites:" << SV_ << std::endl;
}

//cout << latitude_ << ", " << longitude_ << endl; // decimal degrees
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
    int strLength = strlen(NS_); //finds length of the array
    for (int i = 0; i < strLength; i++) {
        NS[i] = NS_[strLength-1-i]; //copies UserInput in reverse to TempInput
    }
    NS[strLength] = '\0'; //adds NULL character at end
}

void GPS::getEastWest(char EW[]) // returns either a East pole or West pole
{
    int strLength = strlen(EW_); //finds length of the array
    for (int i = 0; i < strLength; i++) {
        EW[i] = EW_[strLength-1-i]; //copies UserInput in reverse to TempInput
    }
    EW[strLength] = '\0'; //adds NULL character at end
}

