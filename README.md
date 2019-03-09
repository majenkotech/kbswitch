K780 Function Key Switcher
==========================

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
