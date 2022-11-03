#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <windows.h>
#include "math.h"

int main()
{
int aX = 78, aY = -429.000000, aZ = 16356;
int gX = 0, gY = 0, gZ =0;
int mgX = 0, mgY = 0, mgZ =0;
float XL_Sensitivity = 0.061; // +/-2g
float G_Sensitivity = 4.375; // +/- 125 dps
float MG_Sensitivity = 0.0015; // at default full scale
double pi = 3.14159265358979;

float aXg = (aX*XL_Sensitivity)/1000; // 0.061 sensitivity at +/-2g result in g
float aYg = (aY*XL_Sensitivity)/1000; // 
float aZg = (aZ*XL_Sensitivity)/1000;

float gXdps = (gX*G_Sensitivity)/1000;
float gYdps = (gY*G_Sensitivity)/1000;
float gZdps = (gZ*G_Sensitivity)/1000;

float A = sqrt(pow(aXg,2)+ pow(aYg,2) + pow(aZg,2)); // force on object
printf("A: %f\n",A);
float XL_xdeg = acos((aXg/A))/(pi/180);
float XL_ydeg = acos((aYg/A))/(pi/180);
float XL_zdeg = acos((aZg/A))/(pi/180);
float XL_anlge = (float)(atan2(aXg,aYg))/(pi/180);
printf("Tangens: %f\n", XL_anlge);

float mgXdps = (mgX*MG_Sensitivity)/1000;
float mgYdps = (mgY*MG_Sensitivity)/1000;
float mgZdps = (mgZ*MG_Sensitivity)/1000;
//magnetometer G unit aka mm/s so have to divide by 1000 again.

printf("aX = %f g, aY = %f g, aZ = %f g\n", aXg, aYg, aZg);
printf("aX = %f g, aY = %f g, aZ = %lf g\n", XL_xdeg, XL_ydeg, XL_zdeg);
printf("gX = %f dps, gY = %f dps, aZ = %f dps\n", gXdps, gYdps, gZdps);
printf("mX = %f m/s, mY = %f m/s, mZ = %f m/s\n", gXdps, gYdps, gZdps);
return 0;
}