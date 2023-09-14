# Kernel and Userspace Interaction with Structures 🚀

## Overview 📜

This project establishes a communication between kernel space and user space using a character device. The main goal is to safely pass a structure (`abc`) back and forth.

## Code Walkthrough 🚶

### Kernel Space 🌐

1. **Setup**:
   - Necessary headers and the definition of the `abc` structure are included.
   - The `dev_t` type variable, `devicenumber`, will store the major and minor number allocated to the device.
   - A `struct cdev` named `mycdev` is declared to represent the character device.
   
2. **Device Operations**:
   - `device_open`: Logs when the device is opened.
   - `device_release`: Logs when the device is closed.
   - `device_read`: Reads from kernel space structure and writes to user space.
   - `device_write`: Reads from user space and writes to the kernel space structure.

3. **Initialization (`test_hello_init`)**:
   - The device class is created.
   - A character device region is allocated dynamically.
   - The character device is initialized, added, and linked with file operations.

4. **Exit (`test_hello_exit`)**:
   - Cleanup is performed. Device and class are destroyed, and the char region is unregistered.

### User Space 🖥️

1. **Setup**:
   - Headers and structure definition are included.
   
2. **Main Function**:
   - Opens the device.
   - Writes the initialized structure to the device.
   - Reads back the structure and prints the result.
   - Closes the device.

---

## Built-in APIs Explained 📘

### Kernel Space:

- `class_create`: Creates a struct class pointer that can later be used in calls to device_create.
- `alloc_chrdev_region`: Dynamically allocates a range of char device numbers.
- `device_create`: Creates a device and registers it with sysfs.
- `cdev_init`: Initializes cdev structure and links it with file operations.
- `cdev_add`: Adds the device to the system, making it live.
- `copy_to_user`: Copies data from kernel space to user space.
- `copy_from_user`: Copies data from user space to kernel space.

### User Space:

- `open`: Opens a file or device.
- `write`: Writes data to a file or device.
- `read`: Reads data from a file or device.
- `close`: Closes a file or device.

---

## Technical Interview Questions 🤓

1. **Q:** Explain the difference between `get_user` and `copy_from_user`.  
   **A:** While both are used to safely transfer data from user space to kernel space, `get_user` is designed for simple data types like `char` or `int`, whereas `copy_from_user` is suitable for transferring larger or complex data, such as structures.

2. **Q:** Why do we need to use functions like `copy_from_user` or `copy_to_user` instead of direct memory access between kernel and user space?  
   **A:** Accessing user space memory directly can lead to unexpected system behavior, crashes, and potential security issues. These functions ensure safe, boundary-checked access, preventing potential violations.

3. **Q:** What happens when you call `cdev_add` in the kernel module?  
   **A:** The `cdev_add` function links the character device (represented by the `struct cdev` instance) to the device number, effectively making it live and accessible from user space.

---

## Design Interview Questions 🎨

1. **Q:** How would you extend this design to support multiple device instances, each with its own `abc` structure?  
   **A:** One way to support multiple device instances is to dynamically allocate device numbers and corresponding structures based on the number of instances required. We would also adjust the file operations to work on the relevant structure based on the accessed device instance.

2. **Q:** How would you redesign this kernel module to support both synchronous and asynchronous communication with user space?  
   **A:** For asynchronous communication, we could use the kernel's "aio" (Asynchronous I/O) operations. This would involve updating the module to handle `aio_read` and `aio_write` operations and potentially using kernel mechanisms like completion or work queues to handle the asynchronous tasks.

3. **Q:** If we wanted to add a notification mechanism such that user space gets notified whenever the `abc` structure changes in kernel space, how would you design it?  
   **A:** A popular mechanism for such notifications is the use of `poll`/`select`. We could implement the `poll` file operation in the kernel module to support this. Additionally, we can utilize the `wait` mechanism in kernel space, allowing the user space to wait until there's a change. Once a change occurs in the `abc` structure, the waiting processes can be awakened.

---

Good luck with your preparations! 🌟

```bash

[root@fedora 24_copy_user]# ./userapp 
Opening File:/dev/mystruct

Write retval:8

Read retval:0    int:5  char:a

Closing File

[root@fedora 24_copy_user]

```