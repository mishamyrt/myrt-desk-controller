all: build

FIRMWARE_PATH = .pio/build/esp32dev/firmware.bin

.PHONY: build
build:
	pio run -e esp32dev

ota: build
	myrt_desk flash_controller $(FIRMWARE_PATH)

.PHONY: espota
espota:
	pio run -t upload

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
