#!/bin/bash
DEVICE_ADDRESS="$1"
DESCRIBE_CMD="curl --connect-timeout 1 --output /dev/null --silent --fail $DEVICE_ADDRESS/describe"
UPDATE_API="$DEVICE_ADDRESS/backlight/update-firmware"
FIRMWARE_PATH="$2"

wait_for_boot() {
    until $($DESCRIBE_CMD); do
        printf '.'
        sleep 0.5
    done
}

wait_for_reset() {
    while $($DESCRIBE_CMD); do
        printf '.'
        sleep 1
    done
}

send_firmware() {
    curl --connect-timeout 1 --output /dev/null --silent --fail \
         -H "Content-Type: application/octet-stream" \
         --data-binary "@$FIRMWARE_PATH" "$UPDATE_API"
}

printf "Waiting for device to be avaliable"
wait_for_boot
printf "\nSending firmware"
send_firmware
printf "\nWaiting for flashing end"
sleep 2
wait_for_reset
printf "\nWaiting for device boot"
wait_for_boot
printf "\nDone"
