all: compile

.PHONY: compile
compile:
	pio run

.PHONY: clear
clear:
	rm -rf .pio

.PHONY: ota
ota:
	pio run -t upload

.PHONY: configure
configure:
	pio init --ide vscode
