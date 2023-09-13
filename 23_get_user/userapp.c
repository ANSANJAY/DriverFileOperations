#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define DEVICE_FILE	"/dev/myint"

int main()
{
	int fd;
	int retval;
	int value = 10;

	printf("Opening File:%s\n", DEVICE_FILE);
	fd = open(DEVICE_FILE, O_RDWR);

	if (fd < 0) {
		perror("Open Failed");
		exit(1);
	}

	getchar();


	retval = write(fd, &value, sizeof(value));
	printf("Write retval:%d\n", retval);
	getchar();

	value = 2;

	retval = read(fd, &value, sizeof(value));
	printf("Read retval:%d\t value:%d\n", retval, value);
	getchar();
	
	printf("Closing File\n");
	close(fd);
	getchar();

	return 0;
}
