BUILD_DIR=build
MODULE_NAME=gpio_driver

obj-m += $(MODULE_NAME).o

GREEN=\033[0;32m
RED=\033[0;31m
NC=\033[0m # No Color

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
	@echo "\n${GREEN}Building success...${NC}\n"
	@if [ -d $(BUILD_DIR) ]; then \
		echo "\n${GREEN}Build directory exists. Moving files to ./build${NC}\n"; \
	else \
		echo "\n${GREEN}Build directory created. Moving files to ./build${NC}\n"; \
		mkdir $(BUILD_DIR); \
	fi
	mv $(MODULE_NAME).ko $(BUILD_DIR)/
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf $(BUILD_DIR)