# rusc-project
Rust-C Abstract Project: Binder for kernel interaction

Rusc-C Abstraction Project is meant to give an abtraction layer for embedded applications. The project is still under development for improvements in code. Feel free to use and tweek around.

## Architecture
Project includes 
* Kernel modules
* C binder application
* Rust application

Devices created by the kernel modules can be access using the binder functions defined in the C application. Rust application utilizes these binders to carry out device operation.
  
```
              +------------------------+
              |    RUST Application    |
              +------------------------+
              |  C Binder Application  |
              +------------------------+
              |     Kernel Module      |
              +------------------------+
              |       Hardware         |
              +------------------------+
```
## Usage

* **Building kernel module**:
Currently only GPIO module is created.
```
cd gpio
make
```
The module gets built and generates ``` .ko ``` object in the build folder in the same directory. Module can be inserted using insmod.

Devices that gets created can be seen in ``` /dev/ ``` path.

GPIO pins that needs to be used can be set in the device tree file (dt folder). Refer <a>https://www.kernel.org/doc/Documentation/devicetree/bindings/pinctrl/</a> for more information on pin configuration.

* **Building the application**:
```
cd application
make           # or make debug to run application with debug log
make release   # to run application without logs
```
The binary gets generated into the build folder. Currently a test application is wrriten named as rust_application. 

* **Pin Mapping**: Pin mappings can be changed using *gpio-mapping.json* file in *application/mapping* folder.

## To do
* UART binder
* I2C binder
