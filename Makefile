.PHONY: compile

compile:
	pio run -e local
flash:
	pio run -e local -t upload
ota:
	pio run -e ota -t upload
	echo "Finish"

rebuild-vscode-intellisense:
	pio init --ide vscode
