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
int b1 = 0b01000000, b2 = 0b01111111;   
x = 16210;
y = 65535;
z = 0;     //Roll & Pitch are the angles which rotate by the axis X and y
float I2C_Data_;
I2C_Data_ = (b1| b2 << 8); // Bitshift registers together
if(I2C_Data_ < 0x8000) 
{

}
else
{
    I2C_Data_ = I2C_Data_ - 0xFFFF;
}
printf("%f\n", I2C_Data_);
return 0;
}