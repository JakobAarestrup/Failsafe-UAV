#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <windows.h>
#include <iomanip>

void convertData(double latitude, double longitude, char NS, char EW)
{

    double lat_Deg = int(latitude) / 100;  // (d)dd(deg)
    double lon_Deg = int(longitude) / 100; // (d)dd(deg)

    double lat_Sec = (latitude - lat_Deg * 100) / 60;  // mm.mmmm(minutes) / 60 = seconds
    double lon_Sec = (longitude - lon_Deg * 100) / 60; // mm.mmmm(minutes) / 60 = seconds

    if (NS == '0' | EW == '0') // is 1 of the arrays empty?
    {
        std::cout << "NS or EW returned N/A. Skipping conversion..." << std::endl;
    }

    else
    {
        if (NS == 'S' & EW == 'E') // handles negative
        {
            latitude = (lat_Deg + lat_Sec) * -1;
            longitude = lon_Deg + lon_Sec;
        }
        else if (NS == 'N' & EW == 'W')
        {

            latitude = lat_Deg + lat_Sec;
            longitude = (lon_Deg + lon_Sec) * -1;
        }
        else if (NS == 'S' & EW == 'W')
        {
            latitude = (lat_Deg + lat_Sec) * -1;
            longitude = (lon_Deg + lon_Sec) * -1;
        }
        else
        {
            latitude = lat_Deg + lat_Sec;
            longitude = lon_Deg + lon_Sec;
        }
        std::cout << std::setprecision(8) << latitude << "," << NS << " " << std::setprecision(9) << longitude << "," << EW << std::endl;
    }
}

int main()
{
    // Number 1 NE Number 2 SE Number 3 SW and Number 4 NW
    double lat[5] = {6936.1774, 519.6205, 425.8334, 4536.9892, 0.0};
    double lon[5] = {14836.9895, 13956.0226, 2520.7232, 10551.1919, 0.0};
    char NESW[] = {'N', 'E', 'S', 'W', '0'};

    char NONE[] = "";

    convertData(lat[0], lon[0], NESW[0], NESW[1]);
    convertData(lat[1], lon[1], NESW[2], NESW[1]);
    convertData(lat[2], lon[2], NESW[2], NESW[3]);
    convertData(lat[3], lon[3], NESW[0], NESW[3]);
    convertData(lat[3], lon[3], NESW[0], NESW[4]);
    convertData(lat[3], lon[3], NESW[4], NESW[0]);

    return 0;
}