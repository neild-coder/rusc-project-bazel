/**
 * @file rust_binder.h
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

#ifndef __RUST_BINDER__
#define __RUST_BINDER__

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

#define IOCTL_SET_VALUE _IOW('m', 1, int)
#define IOCTL_GET_VALUE _IOR('m', 2, int)
#define GPIO_SET_VALUE _IOR('m', 3, int)
#define GPIO_GET_VALUE _IOR('m', 4, int)

// Enable Logging
#ifdef LOGGING
#define LOG(fmt, ...)  printf("[C LOGGING][%s] " fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)  {}
#endif

#define SUCCESS     0
#define FAIL       -1

#define __CMAIN     main()

typedef struct gpio_value_pool{
    int fd;
    int write_value;
    int read_value;

    char* file_name;
} resource_pool;

int gpio_open_binder(resource_pool* gpio_res);
void gpio_close_binder(resource_pool* gpio_res);
int gpio_write_binder(resource_pool* gpio_res);
int gpio_read_binder(resource_pool* gpio_res);

#endif