#!/bin/sh
[ "$(whoami)" != "root" ] && exec sudo -- "$0" "$@"
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="03eb", ATTR{idProduct}=="2140", MODE:="0666"' >> /etc/udev/rules.d/50-usb-scale.rules
