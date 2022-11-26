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

    char *start_ptr, *end_ptr, *start_ptr_origin, *jump_ptr;

    double latitude, longitude;
    int SV_;
    int i = 0;
    for (i = 0; i < 1000; i++)
    {
        /* char gps_string[100] = "GPGGA,12121001,1200.201021,N,01011.51302,E,1,09,0.89,74.9,M,42.8,M,,*64";
        char *gps = gps_string; */
        char gps_string2[100] = "183251.10,,,,,0,00,99.99,,,,,,*62*62";
        char *gps_2 = gps_string2;

        start_ptr = strchr(gps_2, ',');     // find start of latitude field
        end_ptr = strchr(++start_ptr, ','); // find end of field...
        latitude = atof(start_ptr);         // Convert char to float & store in variable

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
        longitude = atof(start_ptr);

        // printf(" lat: %f D:%s long: %f\n",latitude,NS_,longitude);

        start_ptr = jump_ptr;               // find start of pole EW field
        end_ptr = strchr(++start_ptr, ','); // find end of field...
        *end_ptr = '\0';                    // and zero terminate
        strcpy(EW_, start_ptr);

        start_ptr = strchr(++end_ptr, ','); // find start of satellite field
        end_ptr = strchr(++start_ptr, ','); // find end of field...
        *end_ptr = '\0';                    // and zero terminate
        SV_ = atoi(start_ptr);              // Convert char to int & store in variable

        printf("latitude: %f %s longitude: %f %s Satelites: %d\n\n", latitude, NS_, longitude, EW_, SV_); //  Convert string to float & store in variable
    }
    return 0;
}