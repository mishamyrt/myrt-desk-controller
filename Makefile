.PHONY: compile

compile:
	pio run
flash:
	pio run -e local -t upload
flash-ota:
	pio run -e ota -t upload

rebuild-vscode-intellisense:
	pio init --ide vscode
