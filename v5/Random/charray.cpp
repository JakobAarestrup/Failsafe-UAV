#include <stdio.h>
#include <unistd.h>

int main()
{
    char buff[10] = "D";
    char c;

    c = buff[0];
    printf("%c", c);

    return 0;
}