#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <windows.h>

int main()
{   
    
    char NS_[1];
    char NS_s[1];
    char EW_[1];
    char gps_string[100] = "GPGGA,,1200.201021,N,01011.51302,E,1,09,0.89,74.9,M,42.8,M,,*64";
    char gps_string2[100] = "GPGGA,,,,,,,,,,,,,,,";
   
    char *gps = gps_string;
    char *gps_2 = gps_string2;

    char *start_ptr;
    char* end_ptr; 
    char* start_ptr_origin ;
    double latitude , longitude;
    int SV_;
    int i = 5;

    start_ptr_origin = strchr(gps, ',');   // find start of field #1
    start_ptr = strchr(++start_ptr_origin, ','); // find start of field #2 - latitude
    end_ptr = strchr(++start_ptr, ',');  // find end of field...  
    latitude = atof(start_ptr);   // Convert char to float & store in variable
    
    start_ptr = strchr(++start_ptr, ',');  // find start of field #3 - NS_
    end_ptr = strchr(++start_ptr, ','); // find end of field... 
    *end_ptr = '\0';   // and zero terminate
    strcpy(NS_, start_ptr);
    
    start_ptr += 2; // field 4
    end_ptr = strchr(++start_ptr, ','); // find end of field...
    *end_ptr = '\0';  // and zero terminate
    longitude = atof(start_ptr); 

    start_ptr = strchr(++end_ptr, ',');
    --start_ptr; // find start of field #5 - EW
    end_ptr = strchr(start_ptr, ','); // find end of field... 
    *end_ptr = '\0';  // and zero terminate

    strcpy(EW_, start_ptr);

    start_ptr = strchr(++end_ptr, ','); // find start of field 7
    end_ptr = strchr(++start_ptr, ','); // find end of field... 
    *end_ptr = '\0';  // and zero terminate
    SV_ = atoi(start_ptr); // Convert char to int & store in variable
    
    printf("latitude: %f %s longitude: %f %s Sattelites: %d\n", latitude, NS_, longitude, EW_, SV_);      //  Convert string to float & store in variable

 return 0;
} 