all: compile

FIRMWARE_PATH = .pio/build/esp32dev/firmware.bin
SRC := \
	$(wildcard include/*.h) \
	$(wildcard src/*.cpp) \
	$(wildcard src/*/*.cpp) \
	$(wildcard src/*/*/*.cpp) \
	$(wildcard src/*/*/*/*/*.cpp) \
	$(wildcard src/*/*/*/*/*/*.cpp) \
	$(wildcard src/*.h) \
	$(wildcard src/*/*.h) \
	$(wildcard src/*/*/*.h) \
	$(wildcard src/*/*/*/*/*.h) \
	$(wildcard src/*/*/*/*/*/*.h) \
	$(wildcard lib/*/*.cpp) \
	$(wildcard lib/*/*/*.cpp) \
	$(wildcard lib/*/*/*/*.cpp) \
	$(wildcard lib/*/*.h) \
	$(wildcard lib/*/*/*.h) \
	$(wildcard lib/*/*/*/*.h) \

compile: $(FIRMWARE_PATH)
.PHONY: flash
flash:
	pio run -t upload

ota: $(FIRMWARE_PATH)
	myrt_desk flash_controller $(FIRMWARE_PATH)

$(FIRMWARE_PATH): $(SRC)
	pio run -e esp32dev

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
