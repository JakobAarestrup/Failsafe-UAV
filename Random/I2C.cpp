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
int aX = 45234, aY = 13181  , aZ = 56912;
aX = aX-0xFFFF;
aY = aY-0xFFFF;
int gX = 0, gY = 0, gZ =0;
int mgX = 0, mgY = 0, mgZ =0;
float XL_Sensitivity = 0.061; // +/-2g
float G_Sensitivity = 4.375; // +/- 125 dps
float MG_Sensitivity = 0.0015; // at default full scale
double pi = 3.14159265358979;
double r2deg =57.29578;

float aXg = (aX*XL_Sensitivity)/1000; // 0.061 sensitivity at +/-2g result in g
float aYg = (aY*XL_Sensitivity)/1000; // 
float aZg = (aZ*XL_Sensitivity)/1000;

float gXdps = (gX*G_Sensitivity)/1000;
float gYdps = (gY*G_Sensitivity)/1000;
float gZdps = (gZ*G_Sensitivity)/1000;

float AccXangle = (atan2(-aXg,-aZg))/(pi/180)+180;
float AccYangle = (atan2(-aYg,-aZg))/(pi/180)+180;
float AccZangle = (float) (atan2(aXg,aYg)+pi)*r2deg;

printf("ACC_X: %f, ACC_Y: %f and ACC_Z: %f\n",AccXangle,AccYangle,AccZangle);
double XL_Roll_  = atan2(aY, aZ) * r2deg;
double XL_Pitch_ = atan2((-aX), sqrt(aY * aY + aZ * aZ)) * r2deg;

printf("Converted - X: %lf, Y: %lf\n\n", XL_Roll_, XL_Pitch_);

float A = sqrt(pow(aXg,2)+ pow(aYg,2) + pow(aZg,2)); // force on object
//printf("A: %f\n",A);
/* float XL_xdeg = acos((aXg/A))/(pi/180);
float XL_ydeg = acos((aYg/A))/(pi/180);
float XL_zdeg = acos((aZg/A))/(pi/180);  */

/* double XL_X = (atan2(-aZg,-aXg)/(pi/180))+180;
double XL_Y = (atan2(-aYg,-aZg)/(pi/180))+180;
double XL_Z = (atan2(-aXg,-aZg)/(pi/180))+180;

printf("Vinkel_X: %f Vinkel_Y: %f, Vinkel_Z: %f\n", XL_X,XL_Y,XL_Z);

float mgXdps = (mgX*MG_Sensitivity)/1000;
float mgYdps = (mgY*MG_Sensitivity)/1000;
float mgZdps = (mgZ*MG_Sensitivity)/1000;
//magnetometer G unit aka mm/s so have to divide by 1000 again.

printf("aX = %f g, aY = %f g, aZ = %f g\n", aXg, aYg, aZg);
//printf("aX = %f deg, aY = %f deg, aZ = %lf deg\n", XL_xdeg, XL_ydeg, XL_zdeg);
printf("gX = %f dps, gY = %f dps, aZ = %f dps\n", gXdps, gYdps, gZdps);
printf("mX = %f m/s, mY = %f m/s, mZ = %f m/s\n", gXdps, gYdps, gZdps); */
return 0;
}