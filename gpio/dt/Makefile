all:
	cpp -P gpio_overlay.dts gpio_overlay_processed.dts
	dtc -@ -I dts -O dtb -o rgpio-overlay.dtbo gpio_overlay_processed.dts