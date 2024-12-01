/// Provides with GPIO functionalities.
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
/// # GPIO Module
/// Provides with GPIO functionalities. Binds C functions for opening, closing, reading and writing to GPIO Pins.
/// 
/// Must be used after gpio devices are created.
/// 
/// ### Example
/// ```
/// use crate::gpio::gpio::GpioResource;
/// 
/// fun gpio_operation(){
///     let gpio_pin = GpioResource::new("gpio1");
///     let err = gpio_pin.open();
///     match err {
///          Ok(_) => log!("Opened device successfully!"),
///          Err(err) => log!("Error opening device! {}", err),
///     }
///     gpio_pin.close();
/// }
/// ```
/// 
/// Writing to GPIO Pin 
/// ```
/// gpio_pin.set(1); // or gpio_pin.set(0) if GPIO configured as output
/// let err = gpio_pin.write();
/// ```
/// 
/// Reading from GPIO Pin.
/// ```
/// gpio_pin.read();
/// let value = gpio_pin.get();
/// ```
/// 
pub mod gpio {
    use std::ffi::CString;
    use std::os::raw::c_char;

    use crate::{log, abstraction::misc::misc};

    /// Structure for storing GPIO data
    /// 
    /// Parameters
    /// * `fd` - File Descriptor
    /// * `write_value` - Value to be written to GPIO Pin
    /// * `read_value` - Value to be read from GPIO Pin
    /// * `file_name` - Device file name example `/dev/<device_name>`
    /// 
    #[repr(C)]
    pub struct GpioResource {
        fd: u32,
        write_value: u32,
        read_value: u32,

        file_name: *mut c_char,
    }

    extern "C" {
        fn gpio_open_binder(gpio_res: *mut GpioResource) -> u32;
        fn gpio_close_binder(gpio_res: *mut GpioResource);
        fn gpio_write_binder(gpio_res: *mut GpioResource) -> u32;
        fn gpio_read_binder(gpio_res: *mut GpioResource) -> u32;
    }

    impl GpioResource {
        /// Creates a new GPIO resource
        /// 
        /// Returns
        /// * `GpioResource` structure
        /// 
        /// Parameters
        /// * `device_file` - Name of the device to be opened. eg: `gpio1` or `gpio2`
        /// 
        /// Example
        /// ```
        /// let gpio_pin = GpioResource::new("gpio1");
        /// ```
        /// 
        pub fn new(device_file: &str) -> GpioResource {
            //let device_file = CString::new(device_file).unwrap();

            let gpio_path = misc::get_device_path(device_file.to_string());
            log!("Gpio_Path: {}", gpio_path);

            let device_file = CString::new(gpio_path).unwrap();

            GpioResource {
                fd: 0,
                write_value: 0,
                read_value: 0,
                file_name: device_file.into_raw(),
            }
        }

        /// Opens GPIO resource. Used as a binder for device open function defined in C.
        /// 
        /// Returns
        /// * `Result<(), u32>` - Indicates success or failure
        /// 
        /// Example
        /// ```
        /// let err = gpio_pin.open();
        /// ```
        ///
        pub fn open(&mut self) -> Result<(), u32> {
            unsafe {
                let err = gpio_open_binder(self as *mut GpioResource);
                if err == 0 {
                    Ok(())
                } else {
                    Err(err)
                }
            }
        }

        /// Closes GPIO resource. Used as a binder for device close function defined in C.
        /// 
        /// Example
        /// ```
        /// gpio_pin.close();
        /// ```
        ///
        pub fn close(&mut self) {
            unsafe {
                gpio_close_binder(self as *mut GpioResource);
            }
        }

        /// Write to a GPIO Pin. Used as a binder for device write function defined in C.
        /// Need to set the GPIO value using `set()` function
        /// 
        /// Returns
        /// * `Result<(), u32>` - Indicates success or failure
        /// 
        /// Example
        /// ```
        /// let err = gpio_pin.write();
        /// ```
        ///
        pub fn write(&mut self) -> Result<(), u32> {
            unsafe {
                let err = gpio_write_binder(self as *mut GpioResource);
                if err == 0 {
                    Ok(())
                } else {
                    Err(err)
                }
            }
        }

        /// Read from a GPIO Pin. Used as a binder for device read function defined in C.
        /// Need to get the GPIO value using `get()` function after reading
        /// 
        /// Returns
        /// * `Result<(), u32>` - Indicates success or failure
        /// 
        /// Example
        /// ```
        /// let err = gpio_pin.read();
        /// ```
        ///
        pub fn read(&mut self) -> Result<(), u32> {
            unsafe {
                let err = gpio_read_binder(self as *mut GpioResource);
                if err == 0 {
                    Ok(())
                } else {
                    Err(err)
                }
            }
        }

        /// Function to set GPIO value. Used when GPIO pin is set as output.
        /// 
        /// Arguments
        /// * `value` - Value to be set 1 or 0.
        /// 
        /// Example
        /// ```
        /// gpio_pin.set();
        /// ```
        ///
        pub fn set(&mut self, value: u32) {
            self.write_value = value;
        }

        /// Function to set GPIO value. Used when GPIO pin is set as input.
        /// 
        /// Returns
        /// * `u32` - Indicates the value read from GPIO.
        /// 
        /// Example
        /// ```
        /// gpio_pin.get();
        /// ```
        ///
        pub fn get(&mut self) -> u32 {
            self.read_value
        }
    }
}
