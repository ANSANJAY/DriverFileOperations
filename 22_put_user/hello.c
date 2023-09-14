// Import necessary kernel headers
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

// Initialize module-level variables
int base_minor = 0;
char *device_name = "jiffies";
int count = 1;
dev_t devicenumber;

// Define device-related structures
static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev;

// Define the module's license type
MODULE_LICENSE("GPL");

// Callback function when device is opened
static int device_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

// Callback function when device is released or closed
static int device_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

// Callback function when the device is read from
static ssize_t device_read(struct file *file, char __user *user_buffer,
                           size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
    // Ensure we have enough space to copy jiffies value
	if (count < sizeof(jiffies))
		return -EINVAL;

    // Use put_user to copy jiffies to user space and return appropriate values based on success or failure
	if (!put_user(jiffies, (u64 *)user_buffer))
		return sizeof(jiffies);
	else
		return -EFAULT;
}

// Callback function when the device is written to (does not perform any specific write operation in this example)
static ssize_t device_write(struct file *file, const char __user *user_buffer,
                            size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	return count;
}

// Define the operations that our character device will support
struct file_operations device_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

// Initialization function to be called when the module is loaded
static int test_hello_init(void)
{
    // Create a class for our character device
	class = class_create(THIS_MODULE, "myclass");

    // Allocate a major and minor number for our device
	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
		printk("Device number registered\n");
		printk("Major number received:%d\n", MAJOR(devicenumber));

        // Create the device
		device = device_create(class, NULL, devicenumber, NULL, device_name);
		
        // Initialize the character device and add it to the system
		cdev_init(&mycdev, &device_fops);
		mycdev.owner = THIS_MODULE;
		cdev_add(&mycdev, devicenumber, count);
	}
	else
		printk("Device number registration Failed\n");

	return 0;
}

// Exit function to be called when the module is unloaded
static void test_hello_exit(void)
{
    // Cleanup: Remove the device, delete the class and character device, and release the device number
	device_destroy(class, devicenumber);
	class_destroy(class);
	cdev_del(&mycdev);
	unregister_chrdev_region(devicenumber, count);
}

// Specify initialization and exit functions
module_init(test_hello_init);
module_exit(test_hello_exit);
