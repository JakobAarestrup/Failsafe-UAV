#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main ()
{
    int count = 0;
    char buff[] = "GPGGA,134658.00,5106.9792,N,11402.3003,W,2,09,1.0,1048.47,M,-16.27,M,08,AAAA*60";
    char* NMEA[15];
   /*  char* latitude = strtok(NULL, ","); // third token
    char* Pole_NS = strtok(NULL, ","); // fourth token
    char* longitude = strtok(NULL, ","); // fifth token
    char* Pole_WS = strtok(NULL, ","); // sixth token
    char* token_2 = strtok(NULL, ","); // seventh token
    char* satellites = strtok(NULL, ","); // eight token */
    char* b1 = strtok(buff,","); // first token
    while (b1 != NULL)
    {
        NMEA[count++] = b1; 
        b1 = strtok(NULL, ",");
        
    }
    char* latitude = NMEA[2];
    char* longitude = NMEA[4];
    char* Pole_NS = NMEA[3];
    char* Pole_WE = NMEA[5];        
    char* satellites = NMEA[7];

    // conversion
    float latitude_ = atof(latitude);
    float longitude_ = atof(longitude);
    int satellites_ = atoi(satellites);

    printf("NMEA: %f %s %f %s %d",latitude_,Pole_NS,longitude_,Pole_WE,satellites_);
    
    return 0;

}

