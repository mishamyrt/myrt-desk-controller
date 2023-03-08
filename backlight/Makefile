all: compile

DEVICE_ADDRESS = 192.168.31.16
FIRMWARE_PATH = .pio/build/nanoatmega328/firmware

.PHONY: compile
compile:
	pio run

.PHONY: ota
ota: compile
	myrt_desk flash_backlight .pio/build/nanoatmega328/firmware.hex

.PHONY: clear
clear:
	rm -rf .pio

.PHONY: configure
configure:
	pio init --ide vscode

.PHONY: lint
lint:
	cpplint \
		--filter=-whitespace/parens,-readability/todo,-build/include_subdir \
		--recursive \
		--linelength 100 \
		src/ lib/ include/
