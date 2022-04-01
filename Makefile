.PHONY: compile

compile:
	pio run
flash:
	pio run -t upload

rebuild-vscode-intellisense:
	pio init --ide vscode
