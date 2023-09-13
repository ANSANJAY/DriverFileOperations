#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd;
	unsigned long long old_jiffies;
	unsigned long long new_jiffies;
	
	fd = open("/dev/jiffies", O_RDWR);
	if (fd < 0) {
		perror("fd failed");
		exit(2);
	}
	if (read(fd, &old_jiffies, sizeof(old_jiffies)) != sizeof(old_jiffies)) {
		printf("Failed in reading first jiffies\n");
		exit(3);
	} else {
		printf("First Read:%lld\n", old_jiffies);
	}
	sleep(1);
	if (read(fd, &new_jiffies, sizeof(new_jiffies)) != sizeof(new_jiffies)) {
		printf("Failed in reading second jiffies\n");
		exit(4);
	} else {
		printf("Second Read:%lld\n", new_jiffies);
	}
	printf("Difference:%lld\n", (new_jiffies - old_jiffies));
	close(fd);
}
