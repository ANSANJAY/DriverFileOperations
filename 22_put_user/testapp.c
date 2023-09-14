#include <stdio.h>   // For standard I/O functions
#include <fcntl.h>   // For file control options
#include <stdlib.h>  // For exit() and other standard library functions

int main(int argc, char *argv[])
{
	int fd;                           // File descriptor for the device
	unsigned long long old_jiffies;   // Store initial jiffies value
	unsigned long long new_jiffies;   // Store subsequent jiffies value
	
	// Open the jiffies character device in read-write mode
	fd = open("/dev/jiffies", O_RDWR);
	
	// Check if opening the device was successful
	if (fd < 0) {
		perror("fd failed");          // Print the error
		exit(2);                      // Exit with error code 2
	}
	
	// Read the jiffies value for the first time
	if (read(fd, &old_jiffies, sizeof(old_jiffies)) != sizeof(old_jiffies)) {
		printf("Failed in reading first jiffies\n");
		exit(3);                      // Exit with error code 3
	} else {
		printf("First Read:%lld\n", old_jiffies);
	}
	
	// Pause the program for 1 second
	sleep(1);
	
	// Read the jiffies value for the second time
	if (read(fd, &new_jiffies, sizeof(new_jiffies)) != sizeof(new_jiffies)) {
		printf("Failed in reading second jiffies\n");
		exit(4);                      // Exit with error code 4
	} else {
		printf("Second Read:%lld\n", new_jiffies);
	}
	
	// Calculate and print the difference between the two readings
	printf("Difference:%lld\n", (new_jiffies - old_jiffies));
	
	// Close the device
	close(fd);
	return 0;                        // End of program
}
