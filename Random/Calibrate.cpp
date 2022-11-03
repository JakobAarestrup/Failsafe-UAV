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
int aX = 78, aY = -429.000000, aZ = 16356;
int gX = 0, gY = 0, gZ =0;
int mgX = 0, mgY = 0, mgZ =0;
float XL_Sensitivity = 0.061; // +/-2g
float G_Sensitivity = 4.375; // +/- 125 dps
float MG_Sensitivity = 0.0015; // at default full scale

float aXg = (aX*XL_Sensitivity)/1000; // 0.061 sensitivity at +/-2g result in g
float aYg = (aY*XL_Sensitivity)/1000; // 
float aZg = (aZ*XL_Sensitivity)/1000;

float gXdps = (gX*G_Sensitivity)/1000;
float gYdps = (gY*G_Sensitivity)/1000;
float gZdps = (gZ*G_Sensitivity)/1000;

float mgXdps = (mgX*MG_Sensitivity)/1000;
float mgYdps = (mgY*MG_Sensitivity)/1000;
float mgZdps = (mgZ*MG_Sensitivity)/1000;
//magnetometer G unit aka mm/s so have to divide by 1000 again.

printf("aX = %f g, aY = %f g, aZ = %f g\n", aXg, aYg, aZg);
printf("gX = %f dps, gY = %f dps, aZ = %f dps\n", gXdps, gYdps, gZdps);
printf("gX = %f m/s, gY = %f m/s, aZ = %f m/s\n", gXdps, gYdps, gZdps);
return 0;
}