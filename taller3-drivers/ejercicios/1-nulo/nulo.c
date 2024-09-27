#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define DEVICE_NAME "nulo"

static struct class *mi_class;
static struct cdev dev;
static dev_t num;

static ssize_t nulo_read(struct file *filp, char __user *data, size_t s,
                         loff_t *off) {
  // Completar
  return 0;
}

static ssize_t nulo_write(struct file *filp, const char __user *data, size_t s,
                          loff_t *off) {
  // Completar
  return s;
}

static struct file_operations nulo_operaciones = {
    // Completar
    .owner = THIS_MODULE,
    .read = nulo_read,
    .write = nulo_write};

static int __init nulo_init(void) {
  cdev_init(&dev, &nulo_operaciones);
  alloc_chrdev_region(&num, 0, 1, "nulo");
  cdev_add(&dev, num, 1);
  mi_class = class_create(THIS_MODULE, DEVICE_NAME);
  device_create(mi_class, NULL, num, NULL, DEVICE_NAME);
  return 0;
}

static void __exit nulo_exit(void) {
  device_destroy(mi_class, num);
  class_destroy(mi_class);
  cdev_del(&dev);
  unregister_chrdev_region(num, 0);
}

// Completar
module_init(nulo_init);
module_exit(nulo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de '/dev/null'");
