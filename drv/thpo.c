#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/time.h>
#include "status.h"

#define DRIVER_AUTHOR "novodim@xiag.ch"
#define DRIVER_DESC "usb sysfs server cooling arduino with ch341 uart-usb bridge based device driver for linux v1.0"

#define CUSTOM_RQ_SET_STATUS    1

/*******************************/
/* baudrate calculation factor */
/*******************************/
#define CH341_BAUDBASE_FACTOR 1532620800
#define CH341_BAUDBASE_DIVMAX 3

enum thpo_hw_type {
	THPO_DEV1,
	THPO_DEV2,
};

static const struct usb_device_id id_table[] = {
	{ USB_DEVICE(0x1a86, 0x7523),
			.driver_info = THPO_DEV1},
	{ },
};

MODULE_DEVICE_TABLE(usb, id_table);

struct thpo_usb {
	struct usb_device	*udev;
	enum thpo_hw_type		type;
	int last_cmd_time;
	int last_read_time;
};

static int ch341_control_out(struct usb_device *dev, u8 request,
                 u16 value, u16 index)
{
    int r;

    dev_dbg(&dev->dev, "ch341_control_out(%02x,%02x,%04x,%04x)\n",
        USB_DIR_OUT|0x40, (int)request, (int)value, (int)index);

    r = usb_control_msg(dev, usb_sndctrlpipe(dev, 0), request,
                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_DIR_OUT,
                value, index, NULL, 0, 1000);

    return r;
}

static int ch341_set_baudrate(struct usb_device *dev,
                          unsigned baud_rate)
{
    short a, b;
    int r;
    unsigned long factor;
    short divisor;

    if (!baud_rate)
        return -EINVAL;
    factor = (CH341_BAUDBASE_FACTOR / baud_rate);
    divisor = CH341_BAUDBASE_DIVMAX;

    while ((factor > 0xfff0) && divisor) {
        factor >>= 3;
        divisor--;
    }

    if (factor > 0xfff0)
        return -EINVAL;

    factor = 0x10000 - factor;
    a = (factor & 0xff00) | divisor;
    b = factor & 0xff;

    r = ch341_control_out(dev, 0x9a, 0x1312, a);
    if (!r)
        r = ch341_control_out(dev, 0x9a, 0x0f2c, b);
    return r;
}

int16_t get_int (uint16_t raw)
{
        int16_t int_part=(raw>>4)-4096;
        if (raw&0x8000) {
        if (raw&0x000f)return -int_part-1; else return -int_part;
        } else return raw>>4;
}

int16_t get_frac (uint16_t raw)
{
        int16_t frac_part=(raw&0x000f);
        //               0                   1
        //               0 1 2 3 4 5 6 7 8 9 0  1  2  3  4  5  6
        int16_t out[17]={0,1,1,2,3,3,4,4,5,6,6, 7, 8, 8, 9, 9, 0};
        if (raw&0x8000) {
                        return out[16-frac_part];
                } else {
                        return out[frac_part];
                        }
}

char get_sign (uint16_t raw){
        printk("raw_gs: %x\n",raw);
        if (raw&0x8000)return '-'; else
        return '+';
}

static ssize_t show_status(struct device *dev, struct device_attribute *attr, char *buf) {
	struct usb_interface *intf = to_usb_interface(dev); 
	struct thpo_usb *led = usb_get_intfdata(intf); 
    int retval;
	int actual_length;
	char * cmd;
    status_t * buffer;
    status_t header_status;
	char * full_status;
    int full_status_len;
	int i,j,loop_count;
	owd_t * ds_id;
	uint16_t * ds_val;
	char * dht;
	int16_t * mpx;
	char * in;
	char buf_tmp[256];
	int buf_tmp_len;

	full_status_len=0;
    printk("show_status called\n");

        buffer = kmalloc(32, GFP_KERNEL);
        if (!buffer) {
                dev_err(&led->udev->dev, "out of memory\n");
                return -1;
        }


    printk("configuring uart\n");
    ch341_set_baudrate(led->udev,9600);
        cmd= kmalloc(2, GFP_KERNEL);
        if (!cmd) {
                dev_err(&led->udev->dev, "out of memory\n");
                return -1;
        }

	cmd[0]='x';
	retval = 0;

        retval = usb_bulk_msg(led->udev,
                 usb_sndbulkpipe(led->udev, 0x82),
                 cmd,
                 1,
                 &actual_length,
                 5000);

    printk("show_status: snd actual_len = %d\n",actual_length);
    printk("show_status: snd retval = %d\n",retval);

        retval = usb_bulk_msg(led->udev,
                 usb_rcvbulkpipe(led->udev, 0x02),
                 buffer,
                 32,
                 &actual_length,
                 5000);
    printk("show_status: rcv actual_len = %d\n",actual_length);
    printk("show_status: rcv retval = %d\n",retval);

/*    if ( actual_length != 32 ) {
            printk("actual_length not equal 32, giving up!\n");
            return retval;
        }
*/
	if(actual_length && retval == 0 )memcpy(&header_status,buffer,sizeof(header_status));
		else {
                  printk("cannot fetch the first chunk retval = %d\n", retval);
                  retval=sprintf(buf, "error\n");
                         kfree(buffer);
                         return retval;
                         }

	if (header_status.status_size == 0) {
            printk("NOT ordering %d bytes for full_status\n",header_status.status_size);
            return retval;
        }



// here we have some more bytes to write into full_status, because we have only parsed header by the moment
    printk("ordering %d bytes for full_status\n",header_status.status_size);

    full_status = kmalloc(header_status.status_size, GFP_KERNEL);
        if (!full_status) {
                dev_err(&led->udev->dev, "out of memory\n");
                return -1;
        }

    printk("copying the first chunk of data (%d bytes) to full_status\n",actual_length);
	memcpy(full_status+full_status_len,buffer,actual_length);
	full_status_len+=actual_length;

	loop_count=0;
	while(full_status_len<header_status.status_size && loop_count<10) {
		loop_count++;
	    printk("getting the next chunk in loop(count - %d, full_status_len - %d of %d)\n",loop_count,full_status_len,header_status.status_size);
        retval = usb_bulk_msg(led->udev,
                 usb_rcvbulkpipe(led->udev, 0x02),
                 buffer,
                 32,
                 &actual_length,
                 5000);
	    printk("show_status:loop rcv actual_len = %d\n",actual_length);
    	printk("show_status:loop rcv retval = %d\n",retval);
		memcpy(full_status+full_status_len,buffer,actual_length);
		full_status_len+=actual_length;
	}
    retval=sprintf(buf, "status_size: %d\nds_info_noBus: %d\nds_info_busSize: %d\nds_busError: %d\ndht_info: %d\ndht_busError: %d\nmpx_info: %d\nmpx_busError: %d\nin_info: %d\nout_info: %d\n",
                                                 header_status.status_size,
                                                 (header_status.ds_info&0xf0)>>4,
                                                 header_status.ds_info&0xf,
                                                 header_status.ds_busError,
                                                 header_status.dht_info,
                                                 header_status.dht_busError,
                                                 header_status.mpx_info,
                                                 header_status.mpx_busError,
                                                 header_status.in_info,
                                                 header_status.out_info
                                                  );
    for (i=0;i<((header_status.ds_info&0xf0)>>4);i++) {
    		printk("show_status:ds:loop i=%d\n",i);
		for(j=0;j<(header_status.ds_info&0xf);j++) {
    		printk("show_status:ds:loop j=%d\n",j);
            ds_id=(owd_t *)(full_status+sizeof(header_status)+i*10*(header_status.ds_info&0xf)+j*8);
            ds_val=(uint16_t *)(full_status+sizeof(header_status)+i*10*(header_status.ds_info&0xf)+(header_status.ds_info&0xf)*8+j*2);
            buf_tmp_len=sprintf(buf_tmp,"ds-%d-%d-%02x%02x%02x%02x%02x%02x%02x%02x: %c%d.%d\n",i,j,
                        ds_id->id[0],
                        ds_id->id[1],
                        ds_id->id[2],
                        ds_id->id[3],
                        ds_id->id[4],
                        ds_id->id[5],
                        ds_id->id[6],
                        ds_id->id[7],
						get_sign(*ds_val),
						get_int(*ds_val),
						get_frac(*ds_val)
                        );      
            retval+=buf_tmp_len;
            strncat(buf,buf_tmp,buf_tmp_len);
		}
	}
    for (i=0;i<header_status.dht_info;i++) {
            printk("show_status:dht:loop i=%d\n",i);
            dht=full_status+sizeof(header_status)+((header_status.ds_info&0xf0)>>4)*10*(header_status.ds_info&0xf)+i*2;
            buf_tmp_len=sprintf(buf_tmp,"dht-%d-temp: %d\ndht-%d-hum: %d\n",i,*dht,i,*(dht+1));
            retval+=buf_tmp_len;
            strncat(buf,buf_tmp,buf_tmp_len);

        }
    for (i=0;i<header_status.mpx_info;i++) {
            printk("show_status:mpx:loop i=%d\n",i);
            mpx=(uint16_t *)(full_status+sizeof(header_status)+((header_status.ds_info&0xf0)>>4)*10*(header_status.ds_info&0xf)+header_status.dht_info*2+i*2);
            buf_tmp_len=sprintf(buf_tmp,"mpx-%d: %d\n",i,*mpx);
            retval+=buf_tmp_len;
            strncat(buf,buf_tmp,buf_tmp_len);

        }
    printk("show_status:in-out i=%d\n",i);
    in=full_status+sizeof(header_status)+((header_status.ds_info&0xf0)>>4)*10*(header_status.ds_info&0xf)+header_status.dht_info*2+header_status.mpx_info*2;
    buf_tmp_len=sprintf(buf_tmp,"in: %d\nout: %d\n",*in,*(in+1));
    retval+=buf_tmp_len;
    strncat(buf,buf_tmp,buf_tmp_len);
	return retval;
} 

static ssize_t set_status(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) { 
	struct usb_interface *intf = to_usb_interface(dev); 
	struct thpo_usb *led = usb_get_intfdata(intf); 
    int retval = 0;
	int actual_length;

        printk("set_status called\n");
		printk("configuring uart\n");
		ch341_set_baudrate(led->udev,9600);
        retval = usb_bulk_msg(led->udev,
                 usb_sndbulkpipe(led->udev, 0x82),
                 (void *)buf,
                 strlen(buf)-1,
                 &actual_length,
                 3000);
                dev_dbg(&led->udev->dev, "retval = %d\n", retval);
	return count; 
} 

static DEVICE_ATTR(status, S_IRUGO | S_IWUSR, show_status, set_status);;


static int thpo_probe(struct usb_interface *interface, const struct usb_device_id *id) {
	struct usb_device *udev = interface_to_usbdev(interface);
	struct thpo_usb *dev = NULL;
	int retval = -ENOMEM;
    struct timeval tv;
	printk("thpo_probe called sizeof status is: %d\n",sizeof(status_t));

	dev = kzalloc(sizeof(struct thpo_usb), GFP_KERNEL);
	if (dev == NULL) {
	dev_err(&interface->dev, "out of memory\n");
	goto error_mem;
	}

 dev->udev = usb_get_dev(udev);
 dev->type = id->driver_info;
 usb_set_intfdata(interface, dev);

do_gettimeofday(&tv);

dev->last_read_time=tv.tv_sec;
dev->last_cmd_time=tv.tv_sec;

retval = device_create_file(&interface->dev, &dev_attr_status);
 if (retval)
  goto error;
	return 0;

error:
 device_remove_file(&interface->dev, &dev_attr_status);
 usb_set_intfdata(interface, NULL);
 usb_put_dev(dev->udev);
 kfree(dev);
 printk("thpo_probe error happen!!!!");
error_mem:
 return retval;
}

static void thpo_disconnect(struct usb_interface *interface) {
    struct thpo_usb *dev;
    dev = usb_get_intfdata(interface);

    device_remove_file(&interface->dev, &dev_attr_status);

    /* first remove the files, then set the pointer to NULL */
    usb_set_intfdata(interface, NULL);

    usb_put_dev(dev->udev);

    kfree(dev);

    dev_info(&interface->dev, "ServerCooling USB disconnected\n");

	printk("thpo_disconnect called");
}

static struct usb_driver thpo_driver = {
	.name =		"thpo",
	.probe =	thpo_probe,
	.disconnect =	thpo_disconnect,
	.id_table =	id_table,
};

module_usb_driver(thpo_driver);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
