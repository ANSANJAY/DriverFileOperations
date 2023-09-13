// Including necessary headers
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

// Define constants and global variables.
int base_minor = 0;            // Base minor number
char *device_name = "mychardev"; // Device name
int count = 1;                // Number of devices
dev_t devicenumber;           // Device number

// Defining structures for device class and cdev
static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev;

MODULE_LICENSE("GPL");        // Licensing for the module

// Define functions for various device operations
static int device_open(struct inode *inode, struct file *file)
{
    pr_info("%s\n", __func__);  // Log the function name
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    pr_info("%s\n", __func__);
    return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer,
                      size_t count, loff_t *offset)
{
    pr_info("%s\n", __func__);
    return 0;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer,
                       size_t count, loff_t *offset)
{
    pr_info("%s\n", __func__);
    return count;
}

// Map the functions to the operations of the character device
struct file_operations device_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

// Module initialization function
static int test_hello_init(void)
{
    // Create a class for the device
    class = class_create(THIS_MODULE, "myclass");

    // Allocate a major and minor number for the device
    if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
        printk("Device number registered\n");
        printk("Major number received:%d\n", MAJOR(devicenumber));

        // Create the device and initialize the cdev structure
        device = device_create(class, NULL, devicenumber, NULL, device_name);
        cdev_init(&mycdev, &device_fops);
        mycdev.owner = THIS_MODULE;
        cdev_add(&mycdev, devicenumber, count);

    }
    else
        printk("Device number registration Failed\n");

    return 0;
}

// Module exit function
static void test_hello_exit(void)
{
    // Cleanup the created resources
    device_destroy(class, devicenumber);
    class_destroy(class);
    cdev_del(&mycdev);
    unregister_chrdev_region(devicenumber, count);
}

// Register the module initialization and exit functions
module_init(test_hello_init);
module_exit(test_hello_exit);
