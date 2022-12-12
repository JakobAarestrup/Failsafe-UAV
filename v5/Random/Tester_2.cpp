#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <windows.h>
#include <math.h>
int main()
{

    FILE *fp;
    int number = 0;
    int N = 0;
    fp = freopen("ASCII.txt", "w", stdout);

    while (1)
    {
        printf("%d = %c\n", number, number);

        number++;
        N++;
        if (N > 255)
        {
            break;
        }
    }

    fclose(fp);

    return 0;
}