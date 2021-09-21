/***************************************************************************//**
*  \file       driver.c
* *******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>
#include <linux/interrupt.h>
#include <asm/io.h>
 
#define SIGETX 44
 
#define REG_CURRENT_TASK _IOW('a','a',int32_t*)
 
#define IRQ_NO 18                       //IRQ 18 es el de mi commputadora! :D
 
/* Signaling to Application */
static struct task_struct *task = NULL;
static int signum = 0;
 
int32_t value = 0;
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
 
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
 
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = etx_read,
        .write          = etx_write,
        .open           = etx_open,
        .unlocked_ioctl = etx_ioctl,
        .release        = etx_release,
};
 
//Manipulador de interrupciones para IRQ 18. 
static irqreturn_t irq_handler(int irq,void *dev_id) {
    struct kernel_siginfo info;
    printk(KERN_INFO "IRQ Compartido: Ha ocurrido una Interrupción");
    
    //Mandando la seña hacia el send
    memset(&info, 0, sizeof(struct kernel_siginfo));
    info.si_signo = SIGETX;
    info.si_code = SI_QUEUE;
    info.si_int = 1;
 
    if (task != NULL) {
        printk(KERN_INFO "Mandando la señal hacia la app\n");
        if(send_sig_info(SIGETX, &info, task) < 0) {
            printk(KERN_INFO "No se puede enviar la señal\n");
        }
    }
 
    return IRQ_HANDLED;
}
 
static int etx_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Archivo del dispositivo Abierto...!!!\n");
    return 0;
}
 
static int etx_release(struct inode *inode, struct file *file)
{
    struct task_struct *ref_task = get_current();
    printk(KERN_INFO "Archivo del dispositivo Cerrado...!!!\n");
    
    //eliminar la tarea
    if(ref_task == task) {
        task = NULL;
    }
    return 0;
}
 
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Leyendo función\n");
    asm("int $0x3B");  //Activación de interrupción. Correspondiente a irq 11
    return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Escribir función\n");
    return 0;
}
 
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if (cmd == REG_CURRENT_TASK) {
        printk(KERN_INFO "REG_CURRENT_TASK\n");
        task = get_current();
        signum = SIGETX;
    }
    return 0;
}
 
 
static int __init etx_driver_init(void)
{
    /*Asignación de número mayor*/
    if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
            printk(KERN_INFO "no se puede asignar un número mayor\n");
            return -1;
    }
    printk(KERN_INFO "Mayor = %d Menor = %d \n",MAJOR(dev), MINOR(dev));
 
    /*Creando la estructura de cdev*/
    cdev_init(&etx_cdev,&fops);
 
    /*Añadir el dispositivo al sistema*/
    if((cdev_add(&etx_cdev,dev,1)) < 0){
        printk(KERN_INFO "No se pudo agregar el servicio al sistema\n");
        goto r_class;
    }
 
    /*Crear el strcut de la clase*/
    if((dev_class = class_create(THIS_MODULE,"etx_class")) == NULL){
        printk(KERN_INFO "No se pudo crear la estructura de la clase\n");
        goto r_class;
    }
 
    /*Crear el dispositivo
    Con el nombre de etx_device*/
    if((device_create(dev_class,NULL,dev,NULL,"etx_device")) == NULL){
        printk(KERN_INFO "No se pudo crear el dispositivo 1\n");
        goto r_device;
    }
 
    if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "etx_device", (void *)(irq_handler))) {
        printk(KERN_INFO "my_device: No se puede registrar el IRQ ");
        goto irq;
    }
 
    printk(KERN_INFO "El driver ha sido inicializado!!!\n");
    return 0;
irq:
    free_irq(IRQ_NO,(void *)(irq_handler));
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev,1);
    return -1;
}
 
static void __exit etx_driver_exit(void)
{
    free_irq(IRQ_NO,(void *)(irq_handler));
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "El driver ha sido eliminado!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
//MODULE_LICENSE("GPL");
//MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
//MODULE_DESCRIPTION("A simple device driver - Signals");
//MODULE_VERSION("1.20");
