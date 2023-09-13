// Including necessary header files
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>

// Static variables for the character device
int base_minor = 0;          // Initial minor number for device
char *device_name = "mychardev"; // Name of the device
int count = 1;               // Number of devices being registered
dev_t devicenumber;          // Will hold the major and minor number

// Declarations for the device class and device itself
static struct class *class;  
static struct device *device;

// Specifying the GPL license for the module
MODULE_LICENSE("GPL");

// Function to open the device
static int device_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

// Function to release the device
static int device_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

// Function to read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer,
                      size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	return 0;
}

// Function to write to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer,
                       size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	return count;
}

// The file operations structure
struct file_operations device_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

// Initialization function for the module
static int test_hello_init(void)
{
	class = class_create(THIS_MODULE, "myclass");

	// Try to allocate a character device number
	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
		printk("Device number registered\n");
		printk("Major number received:%d\n", MAJOR(devicenumber));
		
		// Create the device with the registered number
		device = device_create(class, NULL, devicenumber, NULL, "mydevice");
	}
	else
		printk("Device number registration Failed\n");

	return 0;
}

// Exit function for the module
static void test_hello_exit(void)
{
	// Unregister everything in reverse order
	unregister_chrdev_region(devicenumber, count);
	device_destroy(class, devicenumber);
	class_destroy(class);
}

// Specifying the initialization and exit functions for the module
module_init(test_hello_init);
module_exit(test_hello_exit);
