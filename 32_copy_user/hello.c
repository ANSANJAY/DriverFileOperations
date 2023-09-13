#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>


int base_minor = 0;
char *device_name = "msg";
int count = 1;
dev_t devicenumber;

static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev;

#define MAX_SIZE        1024
char kernel_buffer[MAX_SIZE];
int buffer_index;
MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	file->f_pos = 0;
	buffer_index = 0;
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
        return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer,
                      size_t read_count, loff_t *offset)
{
	int retval;

	pr_info("%s read offset:%lld\n", __func__, *offset);
	if (buffer_index + read_count > MAX_SIZE) {
		pr_err("%s: buffer_index:%d\t read_count:%lu\t Max Size:%d\n",__func__,
				buffer_index, read_count, MAX_SIZE);
		return -ENOSPC;
	}

	retval = copy_to_user(user_buffer, kernel_buffer+*offset, read_count);
	pr_info("%s: Copy to user returned:%d\n", __func__, retval);
	//update file offset
	*offset += read_count;

        return read_count;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer,
                       size_t write_count, loff_t *offset)
{
	int retval;

	pr_info("%s write offset:%lld\n", __func__, *offset);
	if (buffer_index + write_count > MAX_SIZE) {
		pr_err("%s: buffer_index:%d\t write_count:%lu\t Max Size:%d\n",__func__,
				buffer_index, write_count, MAX_SIZE);
		return -ENOSPC;
	}

	retval = copy_from_user(kernel_buffer+buffer_index, user_buffer, write_count);
	pr_info("%s: Copy from user returned:%d\n", __func__, retval);
	pr_info("%s: kernel_buffer:%s\n", __func__, kernel_buffer);

	buffer_index += write_count;
	//update file offset
	*offset += write_count;
        return write_count;
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

		device = device_create(class, NULL, devicenumber, NULL, device_name);
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
