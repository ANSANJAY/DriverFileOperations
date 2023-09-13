#include <linux/kernel.h>     // Required for kernel-level operations.
#include <linux/module.h>     // Needed for all modules.
#include <linux/kdev_t.h>     // For major and minor numbers.
#include <linux/fs.h>         // For file system operations.
#include <linux/device.h>     // Supports driver model.
#include <linux/cdev.h>       // Represents char devices inside the kernel.

// Static (or default) values for the device.
int base_minor = 0;
char *device_name = "mychardev";
int count = 1;
dev_t devicenumber;         // Will store major and minor number.

// Device class and device structures.
static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev;  // Represents char driver.

MODULE_LICENSE("GPL");      // License of the module.

// Called when the device file is opened.
static int device_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);

	//struct file
	if ((file->f_flags & O_ACCMODE) == O_RDONLY) {
		pr_info("Opened File in Read only mode\n");
	}else if ((file->f_flags & O_ACCMODE) == O_WRONLY) {
		pr_info("Opened File in Write only mode");
	}
	else if ((file->f_flags & O_ACCMODE) == O_RDWR) {
		pr_info("Opened File in Read/Write mode");
	}

	if (file->f_flags & O_CREAT) {
		pr_info( "Create if it does not exist");
	}

	if (file->f_flags & O_EXCL) {
		pr_info( "Provide exclusive access");
	}

	if (file->f_flags & O_TRUNC) {
		pr_info( "Truncate the file to zero size first");
	}

	if (file->f_flags & O_APPEND) {
		pr_info( "Append to the file (don't overwrite)");
	}

	if (file->f_flags & O_NONBLOCK) {
		pr_info( "Access methods are non-blocking");
	}
	if (file->f_flags & O_SYNC) {
		pr_info( "O_SYNC");
	}

	pr_info("File Offset:%llu\n", file->f_pos);

	//struct inode
	pr_info("Mode: %ho\n", inode->i_mode);
	pr_info("User Id: %d\n", inode->i_uid.val);
	pr_info("Group Id: %d\n", inode->i_gid.val);
	pr_info("Inode number:%lu\n", inode->i_ino);
	pr_info("Major number:%d\t Minor Number:%d\n", imajor(inode), iminor(inode));
	pr_info("Major number:%d\t Minor Number:%d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev));

	//Accessing struct inode from struct file
	pr_info("Inode number through file structure:%lu\n", file->f_inode->i_ino);
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



struct file_operations device_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};


static int test_hello_init(void)
{
	class = class_create(THIS_MODULE, "myclass");

	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
		printk("Device number registered\n");
		printk("Major number received:%d\n", MAJOR(devicenumber));

		device = device_create(class, NULL, devicenumber, NULL, "mydevice");
		cdev_init(&mycdev, &device_fops);
		mycdev.owner = THIS_MODULE;
		cdev_add(&mycdev, devicenumber, count);

	}
	else
		printk("Device number registration Failed\n");

	return 0;
}

static void test_hello_exit(void)
{
	device_destroy(class, devicenumber);
	class_destroy(class);
	cdev_del(&mycdev);
	unregister_chrdev_region(devicenumber, count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
