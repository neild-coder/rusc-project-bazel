/**
 * @file rust_binder.c
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

#include "rust_binder.h"

/**
 * @brief Function definition provided in lib.rs file. Provides method to run Rust application.
 */
extern void rust_main();

/**
 * @brief C binder function for file open. Used in Rust.
 * @param gpio_res Holds data for device file operation.
 * @return SUCCESS or FAIL.
 */
int gpio_open_binder(resource_pool* gpio_res){
    gpio_res->fd = open(gpio_res->file_name, O_WRONLY);
    
    if(gpio_res->fd < 0){
        LOG("Opening device %s not possible!", gpio_res->file_name);
        return FAIL;
    }
    
    LOG("File descriptor value: %d", gpio_res->fd);

    return SUCCESS;
}

/**
 * @brief C binder function for file close. Used in Rust.
 * @param gpio_res Holds data for device file operation.
 */
void gpio_close_binder(resource_pool* gpio_res){
    close(gpio_res->fd);

}

/**
 * @brief C binder function for ioctl file write. Used in Rust.
 * @param gpio_res Holds data for device file operation.
 * @return SUCCESS or FAIL.
 */
int gpio_write_binder(resource_pool* gpio_res){
    if(ioctl(gpio_res->fd, GPIO_SET_VALUE, &gpio_res->write_value) < 0){
        LOG("Failed to set value! Error: %s", strerror(errno));
        return FAIL;
    }
    
    LOG("Value written successfully!");
    
    return SUCCESS;
}

/**
 * @brief C binder function for ioctl file read. Used in Rust.
 * @param gpio_res Holds data for device file operation.
 * @return SUCCESS or FAIL.
 */
int gpio_read_binder(resource_pool* gpio_res){
    if (ioctl(gpio_res->fd, GPIO_GET_VALUE, &gpio_res->read_value) < 0) {
        LOG("Failed to get value! Error: %s", strerror(errno));
        return FAIL;
    }
    
    LOG("Value read successfully!");
    
    return SUCCESS;
}