#ifndef __RUST_IOCTL__
#define __RUST_IOCTL__

#define TAG "RUSC_GPIO"
#define DEVICE_NAME "r_gpio"
#define DEVICE_CLASS "rust_devices"
#define IOCTL_SET_VALUE _IOW('m', 1, int)
#define IOCTL_GET_VALUE _IOR('m', 2, int)
#define GPIO_SET_VALUE _IOR('m', 3, int)
#define GPIO_GET_VALUE _IOR('m', 4, int)

#define NUMBER_OF_DEVICES   4

#if 1
#define kernel_info(fmt, ...) \
    printk(KERN_INFO "["TAG"]""[%s] " fmt "\n", __func__, ##__VA_ARGS__) 
#else
#define kernel_info(fmt, ...) {}
#endif

#endif