#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <windows.h>

void convertData(double latitude, double longitude, char NS[], char EW[])
{

    double lat_Deg = int(latitude) / 100;  // (d)dd(deg)
    double lon_Deg = int(longitude) / 100; // (d)dd(deg)

    double lat_Sec = (latitude - lat_Deg * 100) / 60;  // mm.mmmm(minutes) / 60 = seconds
    double lon_Sec = (longitude - lon_Deg * 100) / 60; // mm.mmmm(minutes) / 60 = seconds

    if ((strcmp(NS, "") == 0) | (strcmp(EW, "") == 0)) // is 1 of the arrays empty?
    {
        std::cout << "NS or EW returned N/A. Skipping conversion..." << std::endl;
    }

    else
    {
        if ((strcmp(NS, "S") == 0) & (strcmp(EW, "E") == 0)) // handles negative
        {
            latitude = (lat_Deg + lat_Sec) * -1;
            longitude = lon_Deg + lon_Sec;
        }
        else if ((strcmp(NS, "N") == 0) & (strcmp(EW, "W") == 0))
        {

            latitude = lat_Deg + (lat_Sec);
            longitude = lon_Deg + (lon_Sec) * -1;
        }
        else if ((strcmp(NS, "S") == 0) & (strcmp(EW, "W") == 0))
        {
            latitude = lat_Deg + (lat_Sec) * -1;
            longitude = lon_Deg + (lon_Sec) * -1;
        }
        else
        {
            latitude = lat_Deg + lat_Sec;
            longitude = lon_Deg + lon_Sec;
        }
        std::cout << "" << latitude << "," << NS[0] << " " << longitude << "," << EW[0] << std::endl;
    }
}

int main()
{
    double latitude, longitude;
    char NESW[10];
    NESW[0] = "N";
    NESW[1] = "E";
    NESW[2] = "S";
    NESW[3] = "W";

    char NONE[10] = "NONE";
    for (int i = 0; i < 5; i++)
    {
    }

    return 0;
}