#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <windows.h>

void getNorthSouth(char NS[])
{   
    char NS_[10] ="N";
    int strLength = strlen(NS_); //finds length of the array
    for (int i = 0; i < strLength; i++) {
        NS[i] = NS_[strLength-1-i]; //copies UserInput in reverse to TempInput
    }
    NS[strLength] = '\0'; //adds NULL character at end
}
int main()
{
char NS[1];
getNorthSouth(NS);
printf("%s",NS);

return 0;
}