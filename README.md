K780 Function Key Switcher
==========================

This is a small perl program for switching between media keys and function keys on the Logitech K780 keyboard.

Rather annoyingly this superb keyboard defaults to the function keys operating as media keys, and Logitech refuse
to even acknowledge that Linux even exists!

Also Solaar doesn't (yet) support this keyboard any further than simply pairing it with the unified receiver.

So after some reverse engineering of the protocol this little program was born.

It simply sends the USB HID Output Report for switching between media keys and function keys, and nothing else.  In
the future it may be expanded to perform other functions.

Usage:

Select media keys:

    kbswitch -m

Select function keys:

    kbswitch -f

If you have multiple unifying receivers you can select it with a device path:

    kbswitch -d 1-1.6 -f

You can also select the first device found using a VID and PID:

    kbswitch -v 046d -p c52b -f

It defaults to the first device with the standard 046d:c52b VID/PID pair found.

You can also add the `-n` flag to send a destop notification through `notify-send`. This is useful if you bind the program
to key combinations to switch between modes.

----

**Historical notes and musings**

----




A new cleaner system has been invented!

A simple udev rule:

    SUBSYSTEM=="hidraw", KERNEL=="hidraw*", ATTRS{idVendor}=="046d", ATTRS{idProduct}=="c52b", RUN+="/usr/sbin/keyswitch"

coupled with a tiny little script /usr/sbin/keyswitch:

    #!/bin/bash

    if [ "x$ACTION" = "xadd" ]; then
        printf '\x10\x02\x0c\x10\x00\x00\x00' > $DEVNAME
    fi

is all that is needed!

----

Everything below this line is the old experimental system
=========================================================

Crude program to send the codes to switch the functionality
of the K780 (and probably other Logitech keyboards too) function
keys.

It's very much a work in progress, and is the result of my own
reverse engineering of the protocol used.

Note: This breaks the reporting of information from the keyboard to
Solaar.  After using this program Solaar still reports the right number
of devices paired, but it no longer lists the keyboard and its status.

----

Currently the method is very crude, and I need to find a better way
of doing it.

The system is based around sending output reports to the keyboard. The
ones used here are:

    10 02 0c 1c 00 00 00

which switches to F key functionality, and

    10 02 0c 1c 01 00 00

which switches to media key functionality.

The problem is I use HIDAPI to send the reports, and opening the device
causes it to become unbound from the usbhid driver, making the keyboard
and any other device on the same receiver inoperable. A new request
to bind the device is then made through /sys/bus/usb/devices/usbhid/bind
but it's far from a good system. A method needs to be found to send the
output reports without having to unbind the device from the driver.

The ideal situation would be for Solaar to implement the ability to
send these codes.
