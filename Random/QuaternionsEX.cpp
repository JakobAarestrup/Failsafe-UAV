#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <windows.h>
#include <math.h>

int main(){

double x_axis = float(x)*0.061/1000;
double y_axis = float(y)*0.061/1000;
double z_axis = float(z)*0.061/1000;


float c1 = cos(yaw / 2);
float c2 = cos(pitch / 2);
float c3 = cos( / 2);
float s1 = sin(heading / 2);
float s2 = sin(attitude / 2);
float s3 = sin(bank / 2);

float qw = c1 c2 c3 - s1 s2 s3;
float qx = s1 s2 c3 +c1 c2 s3;
float  = s1 c2 c3 + c1 s2 s3;
qz = c1 s2 c3 - s1 c2 s3;



double yaw = atan2(2*qy*qw-2*qx*qz , 1 - 2*qy2 - 2*qz2)
double pitch = asin(2*qx*qy + 2*qz*qw)
double roll = atan2(2*qx*qw-2*qy*qz , 1 - 2*qx2 - 2*qz2)
return 0;
}