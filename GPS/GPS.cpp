#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringSerial.h>

int main() {
  int fd;
  if ((fd = serialOpen("/dev/ttyAMA0",9600))<0) {
    printf("FAIL");
    return 0;
  }
    printf("SUCCESS\n");
  while(1){
    printf("%d\n",serialGetchar(fd));
    fflush(stdout);
    sleep(1);
  }
  return 0;
}