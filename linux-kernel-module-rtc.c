#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/rtc.h>   
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFSIZE 100

MODULE_AUTHOR("Manasseh M. Banda."); 
MODULE_LICENSE("Dual BSD/GPL"); 

static struct proc_dir_entry * ent;

int read_the_time(struct rtc_time * the_time){
    int ret;
    struct rtc_device * the_device = NULL;
    the_device = rtc_class_open(CONFIG_RTC_HCTOSYS_DEVICE);
    ret = rtc_read_time(the_device, the_time);
    rtc_class_close(the_device);
    if(ret < 0){
        printk( KERN_DEBUG "There was an error while reading the time.\n");
        return -1;
    }
    return 0;
}

static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) {
    int ret;
    int num, len, hr, min, sec;
    char buffer[BUFFSIZE];
    struct rtc_device * the_device = NULL;
    struct rtc_time the_time;
    ret = read_the_time(&the_time);

    if(ret < 0){
        return -EFAULT;
    }

    the_device = rtc_class_open(CONFIG_RTC_HCTOSYS_DEVICE);
    if(*ppos > 0 || count > BUFFSIZE){
        return -EFAULT;
    }

    if(copy_from_user(buffer, ubuf, count)){
        return -EFAULT;    
    }

    num = sscanf(buffer,"%d:%d:%d",&hr,&min,&sec);
    if(num != 3){
        return -EFAULT;
    }

    the_time.tm_hour = hr;
    the_time.tm_min = min;
    the_time.tm_sec = sec;
    ret = rtc_set_time(the_device, &the_time);
	printk( KERN_DEBUG "HH: %d, MM: %d, SS: %d\n", the_time.tm_hour, the_time.tm_min, the_time.tm_sec);
    len = strlen(buffer);
    *ppos = len;
    rtc_class_close(the_device);

	return len;
}

static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) {
    char buffer[BUFFSIZE];
    int ret;
    int len = 0;
    struct rtc_time the_time;
    ret = read_the_time(&the_time);
    if(ret < 0){
        return -EFAULT;
    }

    if(*ppos > 0 || count < BUFFSIZE){
        return 0;
    }

    len += sprintf(buffer, "%d:%d:%d\n", the_time.tm_hour, the_time.tm_min, the_time.tm_sec);
    if(copy_to_user(ubuf, buffer, len)){
        return -EFAULT;
    }

    *ppos = len;
    return len;
}

static struct file_operations myops = {
    .owner = THIS_MODULE,
    .read = myread,
    .write = mywrite,
};

static int simple_init(void){
    ent = proc_create("clock", 0660, NULL, &myops);
    return 0;
}

static void simple_cleanup(void){
    proc_remove(ent);
}

module_init(simple_init);
module_exit(simple_cleanup);