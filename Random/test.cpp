
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cstddef>
#include <unistd.h>
#include <iostream>
int main()
{
int b1 = 0b01110000;
int b2 = 0b11111111;
int x = b1|b2 << 8;
printf("%d",x);
return 0;
}
