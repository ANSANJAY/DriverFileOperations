#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define DEVICE_FILE	"/dev/mychardev"  // Define the path to the device file

int main()
{
	int fd;                   // File descriptor for the device
	int retval;               // Return value for system calls
	char buffer[10];          // Buffer to hold data read from the device

	printf("Opening File:%s\n", DEVICE_FILE);
	fd = open(DEVICE_FILE, O_RDWR);  // Open the device file for read and write

	if (fd < 0) {             // Check for errors during file open
		perror("Open Failed"); 
		exit(1);              // Exit with an error code
	}

	getchar();                // Wait for user input 

	retval = write(fd, "hello", 5); // Write "hello" to the device
	printf("Write retval:%d\n", retval); // Print number of bytes written
	getchar();

	retval = read(fd, buffer, 10);  // Read from the device into buffer
	printf("Read retval:%d\n", retval); // Print number of bytes read
	getchar();
	
	printf("Closing File\n");
	close(fd);               // Close the device file
	getchar();

	return 0;
}
