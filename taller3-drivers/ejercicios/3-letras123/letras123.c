#include <linux/cdev.h>   // Char devices
#include <linux/device.h> // Nodos filesystem
#include <linux/fs.h>     // File operations
#include <linux/init.h>
#include <linux/kernel.h>  // Kernel cosas
#include <linux/module.h>  // THIS_MODULE
#include <linux/random.h>  // get_random_bytes
#include <linux/slab.h>    // kmalloc
#include <linux/uaccess.h> // copy_to_user

// Completar

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Generador de letras");
