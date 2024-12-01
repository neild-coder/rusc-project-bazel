/// Creates an external C function to be used from C application.
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
/// # Rust library file
///
/// Creates an external C function to be used from C application.
/// Includes modules that have C binders for kernel operations and application code.
///
/// **Do not change until required**


mod rust_application;
use rust_application::rust_application;

pub mod abstraction{
    pub mod gpio;
    pub mod misc;
}



/// Rust entry point for C application
///
/// Behavior
/// - Called from C
/// - Performs initialization
/// - No return value
#[no_mangle]
pub extern "C" fn rust_main() {
    rust_application();
}
