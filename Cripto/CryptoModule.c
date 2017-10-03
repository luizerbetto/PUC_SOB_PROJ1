#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <asm/uaccess.h> 
#include <linux/device.h>

#define DEVICE_NAME "crypto"   /* Dev name as it appears in /proc/devices   */
#define CLASS_NAME "crypto"
#define BUF_LEN 80              /* Max length of the message from the device */

// Prototypes - this would normally go in a .h file
//static int cryptomodule_init(void);
//static void cryptomodule_exit(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static char *key = "blah";
static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/* Is device open? Used to prevent multiple access to device */

static char msg[BUF_LEN];       /* The msg the device will give when asked */
static char *msg_Ptr;

static char   message[256] = {0};           ///< Memory for the string that is passed from userspace
static short  size_of_message;              ///< Used to remember the size of the string stored

static struct class*  criptoClass  = NULL; ///< The device-driver class struct pointer
static struct device* criptoDevice = NULL; ///< The device-driver device struct pointer

//respostas através do arquivo de dispositivo /dev/crypto
//c-cifrar(write) d-decrifrar(read) h-resumo criptografico(?)
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};


module_param(key, charp, 0000);
MODULE_PARM_DESC(key, "key = ");

static int __init cryptomodule_init(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}
	
	//usar a key dentro de um programa que vai conversar com o módulo
	printk(KERN_INFO "my secret key is: %s\n", key);
	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	
	// Register the device class
   criptoClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(criptoClass)){                // Check for error and clean up if there is
      unregister_chrdev(Major, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(criptoClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "Cripto: device class registered correctly\n");
 
   // Register the device driver
   criptoDevice = device_create(criptoClass, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);
   if (IS_ERR(criptoDevice)){               // Clean up if there is an error
      class_destroy(criptoClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(Major, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(criptoDevice);
   }

	return 0;
}

static void __exit cryptomodule_exit(void)
{
	printk(KERN_INFO "Crypto Module terminated\n");
	device_destroy(criptoClass, MKDEV(Major, 0));     // remove the device
   	class_unregister(criptoClass);                          // unregister the device class
   	class_destroy(criptoClass);                             // remove the device class
   	unregister_chrdev(Major, DEVICE_NAME);             // unregister the major number
   	printk(KERN_INFO "Cripto: Goodbye from the LKM!\n");
}

static ssize_t device_write(struct file *filp,const char *buff,size_t len,loff_t * off)
{
	sprintf(message, "%s(%zu letters)", buff, len);   // appending received string with its length
	size_of_message = strlen(message);                 // store the length of the stored message
	printk(KERN_INFO "EBBChar: Received %zu characters from the user\n", len);
	return len;
}

static ssize_t device_read(struct file *filp,   /* see include/linux/fs.h   */
                           char *buffer,        /* buffer to fill with data */
                           size_t length,       /* length of the buffer     */
                           loff_t * offset)
{
	int error_count = 0;
	// copy_to_user has the format ( * to, *from, size) and returns 0 on success
	error_count = copy_to_user(buffer, message, size_of_message);
	if (error_count==0){            // if true then have success
		printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", size_of_message);
		return (size_of_message=0);  // clear the position to the start and return 0
	}
	else {
		printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
		return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
	}
}

static int device_release(struct inode *inode, struct file *file)
{
        Device_Open--;          /* We're now ready for our next caller */

        /*
         * Decrement the usage count, or else once you opened the file, you'll
         * never get get rid of the module.
         */
        module_put(THIS_MODULE);

        return 0;
}

static int device_open(struct inode *inode, struct file *file)
{
        static int counter = 0;

        if (Device_Open)
                return -EBUSY;

        Device_Open++;
        sprintf(msg, "I already told you %d times Hello world!\n", counter++);
        msg_Ptr = msg;
        try_module_get(THIS_MODULE);

        return 0;
}


module_init(cryptomodule_init);
module_exit(cryptomodule_exit);

MODULE_LICENSE("GPL");
