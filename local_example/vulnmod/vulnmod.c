#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/random.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

// module attributes
MODULE_LICENSE("GPL"); // this avoids kernel taint warning
MODULE_DESCRIPTION("Dummy Char Device Driver");
MODULE_AUTHOR("acez");

// protoypes,else the structure initialization that follows fail
static int on_open(struct inode *, struct file *);
static int on_rls(struct inode *, struct file *);
static ssize_t on_read(struct file *, char *, size_t, loff_t *);
static ssize_t on_write(struct file *, const char *, size_t, loff_t *);

// structure containing callbacks
static struct file_operations fops = 
{
    .read = on_read, // address of on_read
    .open = on_open,  // address of on_open
    .write = on_write, // address of on_write 
    .release = on_rls, // address of on_rls
};


// called when module is loaded, similar to main()
int init_module(void)
{
    int t = register_chrdev(1337,"dummy",&fops); //register driver with major:1337

    if (t < 0) printk(KERN_INFO "dummy: Couldn't register device failed..\n");
    else printk(KERN_INFO "dummy: Successfully registered device...\n");

    return t;
}


// called when module is unloaded, similar to destructor in OOP
void cleanup_module(void)
{
    unregister_chrdev(1337,"ranchar");
}


// called when 'open' system call is done on the device file
static int on_open(struct inode *inod,struct file *fil)
{   
    printk(KERN_INFO "Device opened\n");
    return 0;
}

// called when 'read' system call is done on the device file
static ssize_t on_read(struct file *filp,char *buff,size_t len,loff_t *off)
{
    return 1; 
}

// called when 'write' system call is done on the device file
static ssize_t on_write(struct file *filp,const char *buff,size_t len,loff_t *off)
{
    size_t siz = len;
    void * where = NULL;
    char * what = NULL;

    if(siz > sizeof(where))
        what = buff + sizeof(where);
    else
        goto end;
    
    copy_from_user(&where, buff, sizeof(where));
    memcpy(where, what, sizeof(void *));

end:
    return siz;
}

// called when 'close' system call is done on the device file
static int on_rls(struct inode *inod,struct file *fil)
{
    printk(KERN_INFO "Device closed\n");
    return 0;
}

