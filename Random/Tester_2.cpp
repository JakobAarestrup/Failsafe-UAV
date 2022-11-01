#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>


int main ()
{
    char def_1[] = "0";
    char def_2[] = "N/A";
    //char* data = def_2;
    char* d1 = def_1;
    char* d2 = def_2;

    //char* arr[10];
    //arr[0] = data;
    int count = 0;
    char buff[] = "GPGGA,,5610.29209,N,01011.51302,E,1,09,0.89,74.9,M,42.8,M,,*64";
    int is_GGA_received_completely= 1;
    int i = 0;
    char* NMEA[15];


    if(is_GGA_received_completely==1)
        {
            printf("\nGGA: %s\n",buff); // kan udkommenteres
            char arr[15];
        
            char* b1 = strchr(buff,","); // first token
            for(i = 0 ; i < 15 ; i++)
            {
                NMEA[i] = b1;
                b1 = strchr(NULL, ",");
                printf("b1 = %s\n",b1); 

            }
            
            char* NS_ = NMEA[3]; 
            char* EW_ = NMEA[5]; 

            printf("NS: %s, EW_%s\n",NS_,EW_); 
            i = 0;
            printf("NMEA:");
            for(i = 0 ; i < 15 ; i++)
            {
            
            printf(" %s",NMEA[i]);
            }

            //conversion
            /* latitude_   = atof(NMEA[2]);
            longitude_  = atof(NMEA[4]);
            SV_ = atoi(NMEA[7]);
            printf("Lat: %f Long: %f SV: %d\n",latitude_,longitude_,SV_);
        
            i = 0; // reset i variable
            count = 0; // reset count variable
            is_GGA_received_completely = 0; // reset GGA receive flag variable */
            usleep(1000000);
        } 
    return 0;
}

