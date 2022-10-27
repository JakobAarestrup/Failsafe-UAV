#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main ()
{
  int serial_port ;
  char data;
  int flag = 0;
  char gps[65];
  char arr[]={'G','P','G','G','A','\0'};
 
  while(1)
  {

    int i = 0; 
    char c = 'G';
    flag = 0;
    if(c[0] == 'G')
    { 
      //data = serialGetchar (serial_port);		// receive character serially
        for(i=0; i<6 ;i++)
        {
          if(c[i]==arr[i])
            {
              flag++;
            }
        }
    }
    printf("Value of array: %d\n", flag);
    
      usleep(1000000);
    }
}

