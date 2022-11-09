all: compile

.PHONY: .pio/build/esp32dev/firmware.bin
.pio/build/esp32dev/firmware.bin:
	pio run -e esp32dev

.PHONY: clear
clear:
	rm -rf .pio

.PHONY: ota
ota: .pio/build/esp32dev/firmware.bin
	myrt_desk flash-controller .pio/build/esp32dev/firmware.bin

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
