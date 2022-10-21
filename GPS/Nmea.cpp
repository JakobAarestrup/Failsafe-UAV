#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringSerial.h>

#include "nmea.h"

int main() {
  
  int fd;
    if ((fd = serialOpen("/dev/ttyAMA0",9600))<0) 
    {
        fprintf(stderr,"FAIL");
        return 1;
    }

  char buff[2048];
  int size, it = 0;

  FILE *file;

  nmeaPOS dpos;
  nmeaINFO info;
  nmeaPARSER parser;

  nmea_zero_INFO(&info);
  nmea_parser_init(&parser);

  file = fdopen(fd,"r");

  for (;;) {
    putchar(serialGetchar(fd));
    while(!feof(file)) {
     
      size = (int)fread(&buff[0],1,100,file);

      nmea_parse(&parser,&buff[0],size,&info);

      nmea_info2pos(&info,&dpos);

      printf("%03d, Lat: %f, Lon: %f, Sig: %d, Fix: %d\n",
            it++, dpos.lat, dpos.lon, info.sig, info.fix
            );
    }
    fseek(file, 0, SEEK_SET);
  }

  nmea_parser_destroy(&parser);

  fclose(file);
  
  return 0;
}