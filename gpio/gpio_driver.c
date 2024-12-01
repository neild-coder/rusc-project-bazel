/**
 * @file gpio_driver.c
 * @author Neil Derick
 * @date 2024-11-09
 * @version 1.0.0
 * @brief This file is a part of RUSC Abstraction Project.
 *
 * Copyright (C) 2024  Neil Derick <neil.derick@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/property.h>

#include "gpio_driver.h"

#define MAKE_DEVICE(DEVICE, INDEX) MKDEV(MAJOR(DEVICE), MINOR(DEVICE) + INDEX)

static dev_t device_num;
static struct class *device_class;

static int major_num = 0;
static int minor_num = 0;

static struct gpio_dt_info
{
    int gpio_count;
    const char *gpio_names[NUMBER_OF_DEVICES];
};

typedef struct gpio_dt_info gpio_info;

static struct rust_device
{
    struct cdev rust_cdev;
    struct gpio_desc *gpiod;
    int stored_value;
};

typedef struct rust_device rdevice;
rdevice *rdev;

static int ioctl_open(struct inode *inode, struct file *filp);
static int ioctl_release(struct inode *inode, struct file *filp);
static long ioctl_operation(struct file *filp, unsigned int cmd, unsigned long arg);
static ssize_t ioctl_read(struct file *filp, char *buffer, size_t len, loff_t *offset);
static ssize_t ioctl_write(struct file *filp, const char *buffer, size_t len, loff_t *offset);

static struct file_operations rust_fops = {
    .owner = THIS_MODULE,
    .read = ioctl_read,
    .write = ioctl_write,
    .unlocked_ioctl = ioctl_operation,
    .open = ioctl_open,
    .release = ioctl_release,
};

static int ioctl_open(struct inode *inode, struct file *filp)
{
    rdevice *rdev;
    rdev = container_of(inode->i_cdev, rdevice, rust_cdev);
    filp->private_data = rdev;

    return 0;
}

static int ioctl_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static long ioctl_operation(struct file *filp, unsigned int cmd, unsigned long arg)
{
    rdevice *rdev = filp->private_data;

    int value = 0;

    switch (cmd)
    {
    case IOCTL_SET_VALUE:
        if (copy_from_user(&rdev->stored_value, (int *)arg, sizeof(int)))
        {
            return -EFAULT;
        }
        kernel_info("Store Value: %d", rdev->stored_value);
        return 0;
    case IOCTL_GET_VALUE:
        if (copy_to_user((int *)arg, &rdev->stored_value, sizeof(int)))
        {
            return -EFAULT;
        }
        kernel_info("User accessed value: %d", rdev->stored_value);
        return 0;
    case GPIO_SET_VALUE:
        if (copy_from_user(&value, (int *)arg, sizeof(int)))
        {
            return -EFAULT;
        }
        gpiod_set_value(rdev->gpiod, value);
        kernel_info("GPIO Value set: %d, Actual Value: %d", value, gpiod_get_value(rdev->gpiod));
        return 0;
    case GPIO_GET_VALUE:
        value = gpiod_get_value(rdev->gpiod);
        if (copy_to_user((int *)arg, &value, sizeof(int)))
        {
            return -EFAULT;
        }
        kernel_info("User accessed gpio value: %d", value);
        return 0;
    default:
        return -EINVAL;
    }
}

static ssize_t ioctl_read(struct file *filp, char *buffer, size_t len, loff_t *offset)
{

    char temp_buffer[32];
    int temp_buffer_len;

    rdevice *rdev = filp->private_data;

    if (*offset > 0)
        return 0;

    if (len < sizeof(temp_buffer))
        return -EINVAL;

    int gpio_value = gpiod_get_value(rdev->gpiod);

    temp_buffer_len = scnprintf(temp_buffer, sizeof(temp_buffer), "%d\n", gpio_value);
    if (temp_buffer_len < 0)
        return -EINVAL;

    if (copy_to_user(buffer, temp_buffer, temp_buffer_len))
    {
        kernel_info("Unable to read value");
        return -EFAULT;
    }

    *offset += temp_buffer_len;

    kernel_info("Value read!");

    return temp_buffer_len;
}

static ssize_t ioctl_write(struct file *filp, const char *buffer, size_t len, loff_t *offset)
{
    char temp_buffer[32];
    int value;

    rdevice *rdev = filp->private_data;

    if (len >= sizeof(temp_buffer))
        return -EINVAL;

    if (copy_from_user(temp_buffer, buffer, len))
    {
        kernel_info("Unable to write value");
        return -EFAULT;
    }

    if (temp_buffer[len - 1] == '\n')
    {
        temp_buffer[len - 1] = '\0';
        len--;
    }
    else
    {
        temp_buffer[len] = '\0';
    }

    if (kstrtoint(temp_buffer, 10, &value) < 0)
    {
        kernel_info("Error converting to integer");
        return -EINVAL;
    }

    rdev->stored_value = value;

    if (value != 0 && value != 1)
    {
        kernel_info("Invalid GPIO value. Must be 0 or 1");
        return -EINVAL;
    }

    gpiod_set_value(rdev->gpiod, value);
    kernel_info("Value written: %d\n", rdev->stored_value);

    return len;
}

int get_gpio_info(struct device_node *np, gpio_info *gpio_dt)
{
    int index = 0;
    // Get GPIO names
    gpio_dt->gpio_count = of_property_count_strings(np, "gpio-names");

    if (gpio_dt->gpio_count < 0)
    {
        return gpio_dt->gpio_count;
    }

    for (index = 0; index < gpio_dt->gpio_count; index++)
    {
        of_property_read_string_index(np, "gpio-names", index, &gpio_dt->gpio_names[index]);
        kernel_info("GPIO %d | Name: %s", index, gpio_dt->gpio_names[index]);
    }
    return 0;
}

static int rust_gpio_probe(struct platform_device *pdev)
{
    int create_itt, remove_itt;

    gpio_info gpio_dt;

    struct device *dev = &pdev->dev;
    struct device_node *np = dev->of_node;

    int gpio_err = get_gpio_info(np, &gpio_dt);
    if (gpio_err < 0)
    {
        if (gpio_err == -EINVAL)
        {
            kernel_info("Property does not exist!");
        }
        else if (gpio_err == -ENODATA)
        {
            kernel_info("Poperty does not have a value!");
        }
        else if (gpio_err == -EILSEQ)
        {
            kernel_info("The string is not null-terminated within the length of the property data!");
        }
        else
        {
            kernel_info("Error! Code: %d", gpio_err);
        }
    }
    else{
        kernel_info("GPIO count: %d", gpio_dt.gpio_count);
    }

    // Allocation for devices

    rdev = kzalloc(NUMBER_OF_DEVICES * sizeof(rdevice), GFP_KERNEL);
    if (!rdev)
    {
        kernel_info("Failed to allocate memory for rdevice structures");
        return -ENOMEM;
    }

    if (alloc_chrdev_region(&device_num, 0, NUMBER_OF_DEVICES, DEVICE_NAME) < 0)
    {
        kernel_info("Failed to allocate character device number");
        return -1;
    }

    major_num = MAJOR(device_num);
    minor_num = MINOR(device_num);

    if ((device_class = class_create(THIS_MODULE, DEVICE_CLASS)) < 0)
    {
        kernel_info("Failed to create Kernel class!");
        goto remove_class;
    }

    for (create_itt = 0; create_itt < NUMBER_OF_DEVICES; create_itt++)
    {

        char gpio_name[32];
        struct gpio_desc *gpiod;

        gpiod = gpiod_get(dev, gpio_dt.gpio_names[create_itt], GPIOD_ASIS);
        if (IS_ERR(gpiod))
        {
            kernel_info("Failed to get GPIO: %s", gpio_dt.gpio_names[create_itt]);
            return PTR_ERR(gpiod);
            goto remove_device;
        }

        rdev[create_itt].gpiod = gpiod;

        cdev_init(&rdev[create_itt].rust_cdev, &rust_fops);

        if ((cdev_add(&rdev[create_itt].rust_cdev, MAKE_DEVICE(device_num, create_itt), 1)) < 0)
        {
            kernel_info("Failed to add device! %d", create_itt);

            remove_itt = create_itt;
            while (--remove_itt >= 0)
            {
                cdev_del(&rdev[create_itt].rust_cdev);
            }

            goto remove_class;
        }

        if ((device_create(device_class, NULL, MAKE_DEVICE(device_num, create_itt), NULL, DEVICE_NAME "%d", create_itt)) < 0)
        {
            kernel_info("Failed to create device node! %d", create_itt);

            remove_itt = create_itt;
            while (--remove_itt >= 0)
            {
                device_destroy(device_class, MAKE_DEVICE(device_num, create_itt));
                cdev_del(&rdev[remove_itt].rust_cdev);
            }

            goto remove_device;
        }

        kernel_info("GPIO driver registered (%d:%d)", major_num, minor_num + create_itt);
    }

    return 0;

remove_device:
    class_destroy(device_class);

remove_class:
    unregister_chrdev_region(device_num, NUMBER_OF_DEVICES);
    kfree(&rdev[create_itt].rust_cdev);
    return -1;
}

static int rust_gpio_remove(struct platform_device *pdev)
{

    int remove_itt = NUMBER_OF_DEVICES;
    while (--remove_itt >= 0)
    {
        if (rdev[remove_itt].gpiod)
        {
            gpiod_put(rdev[remove_itt].gpiod);
            rdev[remove_itt].gpiod = NULL;
        }
        device_destroy(device_class, MAKE_DEVICE(device_num, remove_itt));
        cdev_del(&rdev[remove_itt].rust_cdev);
    }
    class_destroy(device_class);
    unregister_chrdev_region(device_num, NUMBER_OF_DEVICES);
    kfree(rdev);

    kernel_info("GPIO driver unregistered");

    return 0;
}

static const struct of_device_id rust_gpio_of_match[] = {
    {.compatible = "raspberrypi,rpi-gpio"},
    {}};

MODULE_DEVICE_TABLE(of, rust_gpio_of_match);

static struct platform_driver rust_gpio_driver = {
    .probe = rust_gpio_probe,
    .remove = rust_gpio_remove,
    .driver = {
        .name = DEVICE_NAME,
        .of_match_table = rust_gpio_of_match,
    },
};

module_platform_driver(rust_gpio_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Neil Derick");
MODULE_DESCRIPTION("A C-Rust GPIO driver");
