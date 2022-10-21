#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

int main() {
  int fd;
  if ((fd = serialOpen("/dev/serial0",9600))<0) {
    fprintf(stderr,"FAIL\n");
    return 1;
  }
  for (;;) {
    printf("%d",serialGetchar(fd));
    fflush (stdout) ;
  }
}