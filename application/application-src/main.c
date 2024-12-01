/**
 * @file main.c
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
 * @brief Main entry point in C
 * @return Returns SUCCESS // Check Header
 */
int __CMAIN{
    
    LOG("Application starting from C");
    
    rust_main();

    return SUCCESS;
}