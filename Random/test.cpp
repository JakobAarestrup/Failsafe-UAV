#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>

using namespace std;

int main()
{

char c = "G";
int c = int(c);
cout << latitude << endl; // decimal degrees
return 0;
}

/* char gdd[] = "5610.29001";
float amount = 01011.51425;

float amount2 = atof(gdd);
float latitude = 0;
float longitude = 0;

int W = 0;
int S = 0;

float lon_1 = int(amount)/100;
float lat_1 = int(amount2)/100; 

float lon_2 = (amount-lon_1*100);
float lat_2 = (amount2-lat_1*100);

cout << lon_1 <<" , " << lon_2 << endl; // (d)dd(deg) mm.mmmm(seconds)
cout << lat_1 <<" , " << lat_2 << endl; // (d)dd(deg) mm.mmmm(seconds)

if (S==1 & W == 0 ) // handles negative
{
    latitude  = (lat_1  + (lat_2/60))*-1;
    longitude = lon_1  + (lon_2/60);
}
else if (S==0 & W == 1)
{
    latitude  = lat_1  + (lat_2/60);
    longitude = (lon_1  + (lon_2/60))*-1;
}
else if(S == 1 & W == 1)
{
    latitude  = (lat_1  + (lat_2/60))*-1;
    longitude = (lon_1  + (lon_2/60))*-1;
}
else
{
    latitude  = lat_1  + (lat_2/60);
    longitude = lon_1  + (lon_2/60);
}

cout << latitude << ", " << longitude << endl; // decimal degrees

return 0; */
