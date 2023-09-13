#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>


#define MAX_DEVICES     5

int base_minor = 0;
char *device_name = "msg";
int count = MAX_DEVICES;
dev_t devicenumber;

static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev[MAX_DEVICES];

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
	int bytes_read;
	int available_space;
	int bytes_to_read;

	pr_info("%s read offset:%lld\n", __func__, *offset);
	 available_space = MAX_SIZE - *(offset);

	if (read_count < available_space)
		bytes_to_read = read_count;
	else
		bytes_to_read = available_space;

	if (bytes_to_read == 0) {
		pr_err("%s: No available space in the buffer for reading\n",
				__func__);
		return -ENOSPC;
	}

	if (buffer_index > *offset)
                bytes_to_read = buffer_index - *offset;
        else
                return 0;


	bytes_read = bytes_to_read - copy_to_user(user_buffer, kernel_buffer+*offset, bytes_to_read);
	pr_info("%s: Copy to user returned:%d\n", __func__, bytes_to_read);

	//update file offset
	*offset += bytes_read;

        return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer,
                       size_t write_count, loff_t *offset)
{
	int bytes_written;
	int available_space;
	int bytes_to_write;

	pr_info("%s write offset:%lld\n", __func__, *offset);
	available_space = MAX_SIZE - *(offset);

	if (write_count < available_space)
		bytes_to_write = write_count;
	else
		bytes_to_write = available_space;

	if (bytes_to_write == 0) {
		pr_err("%s: No available space in the buffer for writing\n",
				__func__);
		return -ENOSPC;
	}

	bytes_written = bytes_to_write - copy_from_user(kernel_buffer+*offset,  user_buffer, bytes_to_write);
	pr_info("%s: Bytes written:%d\n", __func__, bytes_written);
	pr_info("%s: kernel_buffer:%s\n", __func__, kernel_buffer);

	//update file offset
	*offset += bytes_written;
	buffer_index += bytes_written;
        return bytes_written;
}

static loff_t device_lseek(struct file *file, loff_t offset, int orig)
{
        loff_t new_pos = 0;

        switch(orig) {
                case 0 : /*seek set*/
                        new_pos = offset;
                        break;
                case 1 : /*seek cur*/
                        new_pos = file->f_pos + offset;
                        break;
                case 2 : /*seek end*/
                        new_pos = MAX_SIZE - offset;
                        break;
        }
        if(new_pos > MAX_SIZE)
                new_pos = MAX_SIZE;
        if(new_pos < 0)
                new_pos = 0;
        file->f_pos = new_pos;
        return new_pos;
}



struct file_operations device_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
	.llseek = device_lseek
};


static int test_hello_init(void)
{
	int i = 0;
	class = class_create(THIS_MODULE, "myclass");

	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
		int major = MAJOR(devicenumber);
		printk("Device number registered\n");
		printk("Major number received:%d\n", major);
		for (i = 0; i < MAX_DEVICES; i++) {
			dev_t tmp_device = MKDEV(major, i);
			device = device_create(class, NULL, tmp_device, NULL, "%s%d", device_name, i);
			cdev_init(&mycdev[i], &device_fops);
			mycdev[i].owner = THIS_MODULE;
			cdev_add(&mycdev[i], tmp_device, 1);
		}

	}
	else
		printk("Device number registration Failed\n");

	return 0;
}

static void test_hello_exit(void)
{
	int major = MAJOR(devicenumber);
	int i = 0;
	dev_t tmp_device;
	for (i = 0; i < MAX_DEVICES; i++) {
		tmp_device = MKDEV(major, i);
		device_destroy(class, tmp_device);
		cdev_del(&mycdev[i]);
	}
	
	class_destroy(class);
	unregister_chrdev_region(devicenumber, count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
