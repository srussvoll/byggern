#!/bin/sh
[ "$(whoami)" != "root" ] && exec sudo -- "$0" "$@"

JTAGICE3Permissions='SUBSYSTEM=="usb", ATTR{idVendor}=="03eb", ATTR{idProduct}=="2140", MODE:="0666'
JTAGICE3PermissionsFile="/etc/udev/rules.d/50-usb-scale.rules"

if [ ! -f "$JTAGICE3PermissionsFile" ] || [ $(grep -q -F '$JTAGICE3Permissions' "$JTAGICE3PermissionsFile") ]
then
    echo $JTAGICE3Permissions >> "$JTAGICE3PermissionsFile"
fi


ArduinoMegaPort="/dev/ttyACM0"
if [ -f "$ArduinoMegaPort" ]
then
    chmod 777 "$ArduinoMegaPort"
fi
