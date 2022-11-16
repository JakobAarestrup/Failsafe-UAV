
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cstddef>
#include <unistd.h>
#include <iostream>
/* int main()
{
float mx = 28.061972;
float my = 9.909383;
float mz = -52.645428;

float magData[3] = {mx,my,mz};

float A[3][3] = { {1.002979, 0.039343, -0.014713}, 
                  {0.039343, 1.019943, -0.006826}, 
                  {-0.014713, -0.006826, 1.014517} };

float b[3] {7.977849,3.137438,-5.371644};

// magCalibX = A[0,:]*(magData[:]-b[:])
float magxC = A[0][0]*mx + A[0][1]*my + A[0][2]*mz;       // A[0,:]*magData[:]
float bxC   = A[0][0]*b[0] + A[0][1]*b[1] + A[0][2]*b[2]; // A[0,:]*b[:]
float magCalibX = magxC-bxC;

// magCalibY = A[1,:]*(magData[:]-b[:])
float magyC = A[1][0]*mx + A[1][1]*my + A[1][2]*mz;       // A[1,:]*magData[:]
float byC   = A[1][0]*b[0] + A[1][1]*b[1] + A[1][2]*b[2]; // A[1,:]*b[:]
float magCalibY = magyC-byC;

// magCalibZ = A[2,:]*(magData[:]-b[:])
float magzC = A[2][0]*mx + A[2][1]*my + A[2][2]*mz;       // A[2,:]*magData[:]
float bzC   = A[2][0]*b[0] + A[2][1]*b[1] + A[2][2]*b[2]; // A[2,:]*b[:]
float magCalibZ = magzC-bzC;



printf("magCalibX: %f magCalibY: %f  magCalibZ: %f non-calibrated", magCalibX,magCalibY,magCalibZ);
return 0;
} */

int main()
{
float mx = 28.061972;
float my = 9.909383;
float mz = -52.645428;

// hard-iron distortion
float bx = 7.977849; 
float by = 3.137438; 
float bz = -5.371644;

// soft-iron distortion
float A[3][3] = { {1.002979, 0.039343, -0.014713}, 
                  {0.039343, 1.019943, -0.006826}, 
                  {-0.014713, -0.006826, 1.014517} };

// formel magdataCalibrated = A(magdata-b)

float mxb = mx - bx; 
float myb = my - by;
float mzb = mz - bz;

float magCalibX = A[0][0]*mxb + A[0][1]*myb + A[0][2]*mzb;  // A[0,:]*(magdata-b)
float magCalibY = A[1][0]*mxb + A[1][1]*myb + A[1][2]*mzb;  // A[1,:]*(magdata-b)
float magCalibZ = A[2][0]*mxb + A[2][1]*myb + A[2][2]*mzb;  // A[2,:]*(magdata-b)


printf("magCalibX: %f magCalibY: %f  magCalibZ: %f non-calibrated", magCalibX,magCalibY,magCalibZ);
return 0;
} 