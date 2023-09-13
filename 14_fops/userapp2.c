#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define DEVICE_FILE	"/dev/mychardev"

int main()
{
	int fd;
	int retval;
	char buffer[10];
	pid_t pid;

	printf("Opening File:%s\n", DEVICE_FILE);
	fd = open(DEVICE_FILE, O_RDWR);

	if (fd < 0) {
		perror("Open Failed");
		exit(1);
	}

	getchar();
	printf("Closing File\n");
	close(fd);
	getchar();

	return 0;
}
