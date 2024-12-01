/// Provides with file opening and JSON parsing functionalities.
/// 
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
/// # Miscellaneous module
/// Provides with file opening and JSON parsing functionalities.
/// 
/// ### Features
/// - Logging macro with tags
/// - Retrieves device file path
/// 
/// ### Example
/// ```
/// use crate::{log, misc};
/// ```
/// or
/// ```
/// use crate::log;
/// log!("Hello!");
/// ```
/// For getting the device file path use
/// ```
/// let path = get_gpio_file_path("gpio1");
/// ```
/// This should return a device path like ```/dev/<device_name>```
pub mod misc {
    use serde::Deserialize;
    // use serde_json::Result;
    use std::collections::HashMap;
    use std::env;
    use std::error::Error;
    use std::fs::File;
    
    /// Simple logging macro
    ///
    /// Arguments
    /// - Supports format string and optional arguments
    ///
    /// Examples
    /// ```
    /// log!("Message");
    /// log!("Value: {}", 42);
    /// ```
    #[macro_export]
    macro_rules! log {
        ($fmt:expr $(, $arg:expr)*) => {
            println!("[RUST LOGGING][{}:{}] {}", file!(), line!(), format!($fmt $(, $arg)*))
        };
    }

    /// GPIO Mapping 
    /// 
    /// Structure for reading from gpio-mapping.json file.
    /// Used with serde_json crate
    /// 
    #[derive(Deserialize)]
    struct GpioMapping {
        gpio_mapping: HashMap<String, String>,
    }

    pub type MyResult<T> = std::result::Result<T, Box<dyn Error>>;
    
    /// Function for getting device file path
    /// 
    /// Returns
    /// * `Result<(), Err()>` - Indicates success or failure
    /// 
    /// Arguments
    /// * `directory_path` - Path of the JSON file 
    /// * `gpio_device` - GPIO device to be used
    /// * `device_path` - Pass by reference for getting the device file path
    /// 
    /// Examples
    /// ```
    /// let result = get_path(path, device, &mut device_path);
    /// ```
    fn get_path(mut directory_path: String, gpio_device: String, device_path: &mut String) -> MyResult<()> {
        let mut temp_device_path = String::new();

        directory_path = directory_path + "/mapping/gpio-mapping.json";
        //log!("{}", directory_path);
        let json_file = File::open(directory_path)?;

        let gpio_devices: GpioMapping = serde_json::from_reader(json_file)?;

        for (gpio, device) in gpio_devices.gpio_mapping.iter() {
            if *gpio == gpio_device {
                temp_device_path = device.to_string();
                //log!("Path -> {}", temp_device_path);
                break;
            } else {
                continue;
            }
        }

        *device_path = temp_device_path;

        Ok(())
    }

    /// Retrieves the file path for a specified GPIO device
    ///
    /// Returns
    /// * `String` - Complete file path for the GPIO device
    ///
    /// Arguments
    /// * `gpio_device` - GPIO device identifier to locate the file path
    ///
    /// Examples
    /// ```
    /// let path = get_gpio_file_path("gpio1");
    /// ```
    pub fn get_device_path(gpio_device: String) -> String {
        let mut directory_path = String::new();
        let mut device_path = String::new();

        let current_dir = env::current_dir();
        match current_dir {
            Ok(current_dir) => {
                directory_path = current_dir.display().to_string();
                //log!("Current {}", current_dir.display())
            }
            Err(e) => log!("Cannot find directory! {}", e),
        }

        let err = get_path(directory_path, gpio_device, &mut device_path);

        match err {
            Ok(_) => log!("Device path: {}", device_path),
            Err(e) => log!("Error reading device path: {}", e),
        }

        device_path
    }
}
