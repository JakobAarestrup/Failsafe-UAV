#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main() {
	int fd, len;
	char text[255];
	struct termios options; /* Serial ports setting */

	fd = open("/dev/ttyS0", O_RDWR | O_NDELAY | O_NOCTTY);
	if (fd < 0) {
		perror("Error opening serial port");
		return -1;
	}

	/* Read current serial port settings */
	// tcgetattr(fd, &options);
	
	/* Set up serial port */
	options.c_cflag = B4800 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	/* Apply the settings */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);

	/* /* Write to serial port */
	/*strcpy(text, "Hello from my RPi\n\r");
	len = strlen(text);
	len = write(fd, text, len);
	printf("Wrote %d bytes over UART\n", len); */

	/* Read from serial port */
	while(1)
	{
	memset(text, 0, 255);
	len = read(fd, text, 255);
	printf("%s", text);
	}


	close(fd);
	return 0;
}