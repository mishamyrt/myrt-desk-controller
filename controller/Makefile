all: compile

.PHONY: compile
compile:
	pio run -e esp32dev

.PHONY: clear
clear:
	rm -rf .pio

.PHONY: ota
ota:
	pio run -e esp32dev -t upload

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
