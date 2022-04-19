BACKLIGHT_DIR = backlight
CONTROLLER_DIR = controller

all: compile

.PHONY: compile
compile:
	$(MAKE) -C $(BACKLIGHT_DIR)
	$(MAKE) -C $(CONTROLLER_DIR)

.PHONY: clear
clear:
	$(MAKE) clear -C $(BACKLIGHT_DIR)
	$(MAKE) clear -C $(CONTROLLER_DIR)

.PHONY: ota
ota:
	$(MAKE) ota -C $(BACKLIGHT_DIR)
	$(MAKE) ota -C $(CONTROLLER_DIR)

.PHONY: configure
configure:
	$(MAKE) configure -C $(BACKLIGHT_DIR)
	$(MAKE) configure -C $(CONTROLLER_DIR)
