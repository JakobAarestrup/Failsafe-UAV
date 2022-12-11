#include <stdio.h>

int main()
{
    char arr[] = "\xB5\x62\x06\x00\x14\x00\x01\x00\x00\x00\xD0\x08\x00\x00\xC0\x12\x00\x00\x03\x00\x03\x00\x00\x00\x00\x00\xCB\xC4";
    // Calculate the size of char array
    size_t size = sizeof(arr) / sizeof(arr[0]);

    // Print the size of char array
    printf("Size of char array is: %ld byte",
           size);

    char arry[] = "\xB5\x62\x06\x08\x06\x00\x64\x00\x01\x00\x01\x00\x7A\x12";
    // Calculate the size of char array
    size_t sizes = sizeof(arry) / sizeof(arry[0]);

    // Print the size of char array
    printf("Size of char array is: %ld byte",
           sizes);

    return 0;
}