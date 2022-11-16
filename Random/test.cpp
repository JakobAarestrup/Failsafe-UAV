
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cstddef>
#include <unistd.h>
#include <iostream>
int main()
{
double lat_Data = 5610.50098;
double lon_Data = 01010.08142;
double lat_Deg = double(int(lat_Data))/100; // (d)dd(deg)
double lon_Deg = double(int(lon_Data))/100; // (d)dd(deg)

double lat_Sec = (lat_Data-lat_Deg*100); // mm.mmmm(minutes)
double lon_Sec = (lon_Data-lon_Deg*100); // mm.mmmm(minutes)
printf("lat_Deg: %lf, lat_Sec: %lf, lon_Deg: %lf, lon_Sec: %lf", lat_Deg, lat_Sec, lon_Deg, lon_Sec);

mx
return 0;
}
