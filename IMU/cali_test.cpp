#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

int main() 
{
float mx = 28.061972;
float my = 9.909383;
float mz = -52.645428;

float magData[3] = {mx,my,mz};

float A[3][3] = {1.002979, 0.039343, -0.014713, 0.039343, 1.019943, -0.006826, -0.014713, -0.006826, 1.014517};
float b[3] = {7.977849, 3.137438, -5.371644};

printf("magData: \f\n",magData);
printf("A: \f\n",A);
printf("b: \f\n",b);

float magC[3] = A*(magData-b);

printf("Calibrated magData: \f",magC);

return 0;
};