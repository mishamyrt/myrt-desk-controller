.PHONY: compile

compile:
	pio run -e local
flash:
	pio run -e local -t upload
flash-ota:
	pio run -e ota -t upload

rebuild-vscode-intellisense:
	pio init --ide vscode
