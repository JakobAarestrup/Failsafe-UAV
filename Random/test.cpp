
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cstddef>
#include <unistd.h>
#include <iostream>
int main()
{
float mx = 28.061972;
float my = 9.909383;
float mz = -52.645428;

//float magData[3] = {mx,my,mz};

float A[3][3] = { {1.002979, 0.039343, -0.014713}, 
                  {0.039343, 1.019943, -0.006826}, 
                  {-0.014713, -0.006826, 1.014517} };

float b[3] {7.977849,3.137438,-5.371644};

float magxC = A[0][0]*mx + A[0][1]*my + A[0][2]*mz;
float bxC   = A[0][0]*b[0] + A[0][1]*b[1] + A[0][2]*b[2];
float magCalibX = magxC-bxC;

float magyC = A[1][0]*mx + A[1][1]*my + A[1][2]*mz;
float byC   = A[1][0]*b[0] + A[1][1]*b[1] + A[1][2]*b[2];
float magCalibY = magyC-byC;

float magzC = A[2][0]*mx + A[2][1]*my + A[2][2]*mz;
float bzC   = A[2][0]*b[0] + A[2][1]*b[1] + A[2][2]*b[2];
float magCalibZ = magzC-bzC;


printf("magCalibX: %f magCalibY: %f  magCalibZ: %f non-calibrated", magCalibX,magCalibY,magCalibZ);
return 0;
}
