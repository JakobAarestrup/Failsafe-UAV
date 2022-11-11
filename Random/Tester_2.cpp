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
int x, y, z;     
x = 16210;
y = 65535;
z = 0; 
                  //three axis acceleration data
double roll = 0.00, pitch = 0.00;       //Roll & Pitch are the angles which rotate by the axis X and y
double x_Buff = float(x)*0.061/1000;
double y_Buff = float(y)*0.061/1000;
double z_Buff = float(z)*0.061/1000;
printf("X: %f Y: %f Z: %f\n",x_Buff,y_Buff,z_Buff);
roll = atan2(y_Buff , z_Buff) * 57.3;
pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
printf("Roll:%f Pitch: %f",roll,pitch);
return 0;
}