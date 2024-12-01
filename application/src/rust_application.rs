/// Detailed Author and Contribution Information
///
/// ## Author
/// **Neil Derick**
/// - Contact: neil.derick@gmail.com
/// - LinkedIn: [Neil Derick](www.linkedin.com/in/neil-derick-682bb3184)
///
/// ## Contribution Details
/// - Initial Creation: 2024-10-05
/// - Last Modified: 2024-11-09
/// - Version: 1.0.0
///
/// ## Licensing
/// Copyright (C) 2024  Neil Derick <neil.derick@gmail.com>
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <https://www.gnu.org/licenses/>.
///
///
/// Rust application file
/// 
/// Write code for rust application incorporating C-Binder functions.
/// 
///

use crate::log;
use crate::abstraction::gpio::gpio::GpioResource;

use std::io;

pub fn rust_application(){
    let mut gpio_out = GpioResource::new("gpio2");
    let mut gpio_in = GpioResource::new("gpio1");

    let err = gpio_out.open();
    match err {
        Ok(_) => log!("Opened device successfully!"),
        Err(err) => log!("Error opening device! {}", err),
    }

    let err = gpio_in.open();
    match err {
        Ok(_) => log!("Opened device successfully!"),
        Err(err) => log!("Error opening device! {}", err),
    }

    let mut value = String::new();

    io::stdin()
        .read_line(&mut value)
        .expect("Failed to read value!");

    let value: u32 = value.trim().parse().expect("Integer value required!");

    gpio_out.set(value);

    let err = gpio_out.write();

    match err {
        Ok(_) => log!("Success writing value!"),
        Err(err) => log!("Error writing value! Error {}", err),
    }

    let err = gpio_in.read();

    match err {
        Ok(_) => {
            let nvalue = gpio_in.get();
            log!("Success reading value! Value {}", nvalue);
        },
        Err(err) => log!("Error reading value! Error {}", err),
    }

    gpio_out.close();
    gpio_in.close();
}