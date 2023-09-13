#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_FILE	"/dev/mydevice"



int main()
{
	int fd;
	int retval;
	char *buffer = malloc(sizeof(char)*10);

	printf("Opening File:%s\n", DEVICE_FILE);
	fd = open(DEVICE_FILE, O_RDWR);

	if (fd < 0) {
		perror("Open Failed");
		exit(1);
	}

	getchar();

	strcpy(buffer, "hello");

	retval = write(fd, buffer, strlen(buffer));
	printf("Write retval:%d\n", retval);
	getchar();

	printf("Closing File\n");
	close(fd);
	getchar();

	return 0;
}
