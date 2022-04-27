all: compile

DEVICE_ADDRESS = 192.168.31.122
FIRMWARE_PATH = .pio/build/nanoatmega328/firmware

.PHONY: compile
compile:
	pio run

ab: compile
	node scripts/hex2ab $(FIRMWARE_PATH).hex

.PHONY: ota
ota: ab
	./scripts/flash-ab.sh $(DEVICE_ADDRESS) $(FIRMWARE_PATH).ab

.PHONY: clear
clear:
	rm -rf .pio

.PHONY: configure
configure:
	pio init --ide vscode

.PHONY: lint
lint:
	cpplint --filter=-whitespace/parens --recursive src/ lib/ include/
