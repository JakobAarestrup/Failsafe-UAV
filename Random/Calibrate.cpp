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
float aXg = (aX*0.061)/1000; // 0.061 sensitivity at +/-2g result in g
float aYg = (aY*0.061)/1000; // 
float aZg = (aZ*0.061)/1000;

printf("aX = %f g, aY = %f g, aZ = %f g\n", aXg, aYg, aZg);
return 0;
}