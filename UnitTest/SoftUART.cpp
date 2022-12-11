#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main()
{
	int fd, len;
	char text[255];
	int count = 0;
	printf("YOYO!\n");
	fd = open("/dev/ttySOFT0", O_RDWR);
	if (fd < 0)
	{
		perror("Error opening serial port");
		return -1;
	}

	tty.setraw(fd);
	/* Read current serial port settings */
	// tcgetattr(fd, &options);

	/*
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;


	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options); */

	/* /* Write to serial port */
	/*strcpy(text, "Hello from my RPi\n\r");
	len = strlen(text);
	len = write(fd, text, len);
	printf("Wrote %d bytes over UART\n", len); */

	/* Read from serial port */
	printf("Serial");
	while (count < 100000)
	{
		memset(text, 0, 255);
		len = read(fd, text, 255);
		printf("%s", text);
		count++;
	}
	close(fd);

	return 0;
}