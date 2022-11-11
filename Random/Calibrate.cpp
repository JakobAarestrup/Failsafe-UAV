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
int aX = 0, aY = 16300, aZ = 16500;
int gX = 0, gY = 0, gZ =0;
int mgX = 0, mgY = 0, mgZ =0;
float XL_Sensitivity = 439483; // +/-2g
float G_Sensitivity = 4.375; // +/- 125 dps
float MG_Sensitivity = 0.0015; // at default full scale
double r_deg = 57.29578;
double pi = 3.14159265358979;
double conv_Rate = 0.005493247882811;
double sheesh = 0.005493247882811;
double aXraw = aX*conv_Rate;

double aYraw = aY*conv_Rate;
//double aZraw = aZ*conv_Rate;

double aXraw_off = (aX+0xFFFF)*conv_Rate;
double aYraw_off = (aY+0xFFFF)*conv_Rate;
//double aZraw_off = aZ*conv_Rate+0xFFFF;

printf("Raw - X: %f, Y: %f \n",aXraw, aYraw);
printf("Raw_off - X: %lf, Y: %lf \n",aXraw_off, aYraw_off);

float aXg = (aX*XL_Sensitivity)/1000; // 0.061 sensitivity at +/-2g result in g
float aYg = (aY*XL_Sensitivity)/1000; // 
float aZg = (aZ*XL_Sensitivity)/1000; // 0.061 sensitivity at +/-2);
double aXf = float(aX);
double aYf = float(aY);
double aZf = float(aZ);

float gXdps = (gX*G_Sensitivity)/1000;
float gYdps = (gY*G_Sensitivity)/1000;
float gZdps = (gZ*G_Sensitivity)/1000;

float A = sqrt(pow(aXg,2)+ pow(aYg,2) + pow(aZg,2)); // force on object
printf("A: %f\n",A);
/* float XL_xdeg = acos((aXg/A))/(pi/180);
float XL_ydeg = acos((aYg/A))/(pi/180);
float XL_zdeg = acos((aZg/A))/(pi/180);  */

double XL_X = (atan2(-aZ,-aX)/(pi/180));
double XL_Y = (atan2(-aY,-aZ)/(pi/180));

double roll =  (atan2(-aYf,-aZf)/(pi/180));
double pitch = (atan2(aXf,-sqrt(aYf*aYf+aZf*aZf))/(pi/180));

double roll2 = atan2(aYf,aZf)*r_deg;
double pitch2 = (atan2(-aXf,sqrt(aYf*aYf+aZf*aZf)))*r_deg;

printf("Vinkel_X: %f Vinkel_Y: %f, Vinkel_Z: %f\n", XL_X,XL_Y);

printf("roll: %f pitch: %lf, ",roll,pitch);
printf("roll: %f pitch: %lf, ",roll2,pitch2);


float mgXdps = (mgX*MG_Sensitivity)/1000;
float mgYdps = (mgY*MG_Sensitivity)/1000;
float mgZdps = (mgZ*MG_Sensitivity)/1000;
//magnetometer G unit aka mm/s so have to divide by 1000 again.

printf("aX = %f g, aY = %f g, aZ = %f g\n", aXg, aYg, aZg);
//printf("aX = %f deg, aY = %f deg, aZ = %lf deg\n", XL_xdeg, XL_ydeg, XL_zdeg);
printf("gX = %f dps, gY = %f dps, aZ = %f dps\n", gXdps, gYdps, gZdps);
printf("mX = %f m/s, mY = %f m/s, mZ = %f m/s\n", gXdps, gYdps, gZdps);
return 0;
}