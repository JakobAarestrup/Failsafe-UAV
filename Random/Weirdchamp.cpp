 #include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>

int main()
{   
    char NS_[1];
    char EW_[1];
    char gps_string[] = "GPGGA,,1200.201021,N,01011.51302,E,1,09,0.89,74.9,M,42.8,M,,*64";
    char *gps = gps_string;
    char *start_ptr, *end_ptr, *start_ptr_origin;

    start_ptr_origin = strchr(gps, ',');   // find start of field #1
    start_ptr = strchr(++start_ptr_origin, ','); // find start of field #2 - latitude
    end_ptr = strchr(++start_ptr, ',');  // find end of field...                   // and zero terminate
    double latitude = atof(start_ptr);           // Convert char to float & store in variable
    
    start_ptr = strchr(++start_ptr, ',');  // find start of field #3 - NS_
    end_ptr = strchr(++start_ptr, ',');
    *end_ptr = 0; 
    strcpy(NS_, start_ptr);
    //printf("%s\n",NS_);
    
    start_ptr = strchr(++end_ptr, ',');  // find start of field #4
    end_ptr = strchr(++start_ptr, ',');  // find end of field...
    float longitude = atof(start_ptr); // Convert char to float & store in variable);

    start_ptr = strchr(++start_ptr, ',');  // find start of field #5 
    end_ptr = strchr(++start_ptr, ',');  // find end of field...
    *end_ptr = 0;
                           // and zero terminate
    printf("latitude: %f %s longitude: %f %s", latitude, NS_ , longitude, start_ptr);

    /* start_ptr = strchr(++start_ptr, ',');  // find start of field #7 - satellites
    end_ptr = strchr(++start_ptr, ',');  // find end of field...
    *end_ptr = 0;                            // and zero terminate
    int SV_ = atoi(start_ptr);        //  Convert string to float & store in variable */
    //printf("latitude: %f ,longitude: %f Sattelites: %d", latitude, longitude,SV_);      //  Convert string to float & store in variable
 return 0;
} 