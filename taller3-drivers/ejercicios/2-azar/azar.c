#include <linux/init.h>
#include <linux/module.h> // THIS_MODULE
#include <linux/kernel.h> // Kernel cosas
#include <linux/fs.h>     // File operations
#include <linux/cdev.h>   // Char devices
#include <linux/device.h> // Nodos filesystem
#include <linux/uaccess.h> // copy_to_user
#include <linux/slab.h>    // kmalloc
#include <linux/random.h>  // get_random_bytes
                           //
#define DEVICE_NAME "azar"

static struct class *mi_class;
static struct cdev dev;
static dev_t major;
static int limit;

static ssize_t azar_read(struct file *filp, char __user *data, size_t s, loff_t *off){
    unsigned int random_num;
    get_random_bytes(&random_num, sizeof(random_num));
    random_num = random_num % limit;
    char buffer[1024];
    int bytes = snprintf(buffer,1024,"%d\n",random_num);
    copy_to_user(data,buffer,bytes);
    return bytes;
}
static ssize_t azar_write(struct file *filp, const char __user *data, size_t s, loff_t *off){
    char *safe_data = kmalloc(s+1,GFP_KERNEL);
    copy_from_user(safe_data, data,s);
    safe_data[s] = '\0';
    int result = kstrtoint(safe_data,10,&limit);
    kfree(safe_data);
    if(result == 0 ){
        return s;
    } else {
        return result;
    }
}

static struct file_operations azar_operaciones = {
	// Completar
	.owner = THIS_MODULE,
	.read = azar_read,
	.write = azar_write
};

static int __init azar_init(void){
    cdev_init(&dev,&azar_operaciones);
    alloc_chrdev_region(&major, 0, 1, DEVICE_NAME);
    cdev_add(&dev,major,1);
    mi_class = class_create(THIS_MODULE,DEVICE_NAME);
    device_create(mi_class,NULL, major, NULL, DEVICE_NAME);
    return 0;
}

static void __exit azar_exit(void){
    device_destroy(mi_class, major);
    class_destroy(mi_class);
    cdev_del(&dev);
    unregister_chrdev_region(major, 0);
}


module_init(azar_init);
module_exit(azar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Generador de números al azar");
