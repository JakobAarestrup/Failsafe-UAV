#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>


int main ()
{
    char def_1[] = "0";
    char def_2[] = "N/A";
    char* data = def_2;

    char* arr[10];
    arr[0] = data;
    printf("data = %s\n",data);
    printf("Arr = %s\n",arr[0]);
    return 0;
}

