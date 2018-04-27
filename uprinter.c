#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/fs.h>
#include"commands.h" /* common header for all ioctl commands */

static struct usb_device *device;
static struct usb_class_driver class;

static int uprinter_open(struct inode *i,struct file *f)
{
    return 0;
}
static int uprinter_close(struct inode *i,struct file *f)
{
    return 0;
}
/* handles all commands from user space */
/* no data transfer required for dot-matrix printing */
static long uprinter_ioctl(struct file *f,unsigned int cmd,unsigned long arg)
{
    int ret_val;
    switch(cmd)
    {
        case MOVE_PAGE_FWD: /* Move page-motor forward by arg steps */
            ret_val=-1;
            while(ret_val<0)
            ret_val=usb_control_msg(device,usb_sndctrlpipe(device,EP_OUT),PAGE_FWD,USB_DIR_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,arg,0,0,0,0);
            break;
        case MOVE_PAGE_BCK: /* Move page-motor back by arg steps */
            ret_val=-1;
            while(ret_val<0)
            ret_val=usb_control_msg(device,usb_sndctrlpipe(device,EP_OUT),PAGE_BCK,USB_DIR_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,arg,0,0,0,0);
            break;
        case MOVE_CHAR_FWD: /* Move char-motor forward by arg steps */
            ret_val=-1;
            while(ret_val<0)
            ret_val=usb_control_msg(device,usb_sndctrlpipe(device,EP_OUT),CHAR_FWD,USB_DIR_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,arg,0,0,0,0);
            break;
        case MOVE_CHAR_BCK: /* Move char-motor backward by arg steps */
            ret_val=-1;
            while(ret_val<0)
            ret_val=usb_control_msg(device,usb_sndctrlpipe(device,EP_OUT),CHAR_BCK,USB_DIR_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,arg,0,0,0,0);
            break;
        case MOVE_PEN_DOWN: /* Move servo motor (pen down)*/
            ret_val=-1;
            while(ret_val<0)
            ret_val=usb_control_msg(device,usb_sndctrlpipe(device,EP_OUT),PEN_DOWN,USB_DIR_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,arg,0,0,0,0);
            break;
        case MOVE_PEN_HOLD: /* Move servo motor (pen up)*/
            ret_val=-1;
            while(ret_val<0)
            ret_val=usb_control_msg(device,usb_sndctrlpipe(device,EP_OUT),PEN_HOLD,USB_DIR_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,arg,0,0,0,0);
            break;
        default:
            printk(KERN_ERR "USB-Printer: Invalid Command");
    }
    return 0;
}
static struct file_operations fops =
{
    .owner=THIS_MODULE,
    .open=uprinter_open,
    .release=uprinter_close,
    .unlocked_ioctl=uprinter_ioctl,
};
static int uprinter_probe(struct usb_interface *interface,const struct usb_device_id *id)
{
    int reg_status;
    printk(KERN_INFO "USB Printer with VID=%04X PID=%04X Found",id->idVendor,id->idProduct); /* detects device */
    device=interface_to_usbdev(interface);
    class.name="usb/uprinter_dev";
    class.fops=&fops; /* fops linking for custom-class driver */
    if((reg_status=usb_register_dev(interface,&class))<0) /* driver registration (hot plugging) */
    {
        printk(KERN_ERR "USB Printer Driver FAIL");
    }
    else
    {
        printk(KERN_INFO "USB Printer Driver ON, Minor = %d",interface->minor);
    }
    return reg_status;
}

static void uprinter_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "USB Printer Driver OFF"); /* detects device removal */
    usb_deregister_dev(interface,&class); /* dynamic driver deregistration */
    printk(KERN_INFO "USB Printer Removed");
}

static struct usb_device_id uprinter_table[] =
{
    {USB_DEVICE(0x16c0,0x05dc)}, /* allowed devices */
    {}
};

MODULE_DEVICE_TABLE(usb,uprinter_table);

static struct usb_driver uprinter_driver =
{
    .name="uprinter_driver",
    .id_table=uprinter_table,
    .probe=uprinter_probe,
    .disconnect=uprinter_disconnect,
};

static int __init uprinter_init(void)
{
    return usb_register(&uprinter_driver); /* interface driver registration */
}

static void __exit uprinter_exit(void)
{
    usb_deregister(&uprinter_driver); /* interface driver deregistration */
}

module_init(uprinter_init);
module_exit(uprinter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sarthi Chugh & Vipin Verma");
MODULE_DESCRIPTION("USB Custom Printer Driver");
