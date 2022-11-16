#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "linalg.h"
using namespace linalg::aliases;

int main() 
{
float mx = 28.061972;
float my = 9.909383;
float mz = -52.645428;

//float magData[3] = {mx,my,mz};

float3 magData {28.061972,9.909383,-52.645428};

//float A[3][3] = {1.002979, 0.039343, -0.014713, 0.039343, 1.019943, -0.006826, -0.014713, -0.006826, 1.014517};
//float b[3] = {7.977849, 3.137438, -5.371644};
float3x3 A {{1.002979, 0.039343, -0.014713}, {0.039343, 1.019943, -0.006826}, {-0.014713, -0.006826, 1.014517}};
float3 b {7.977849,3.137438,-5.371644};

float3 magC = dot(A,(magData-b));

printf("Calibrated magData: \f",magC);

return 0;
};