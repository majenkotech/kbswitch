#include <stdio.h>
#include <hidapi/hidapi.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: kbswitch [0/1]\n");
        return -1;
    }

    hid_init();
    char path[20];

    int busno = 0;
    int devno = 0;

    struct hid_device_info *cur_dev;
    struct hid_device_info *devices;
    devices = hid_enumerate(0x046d, 0xc52b);
    cur_dev = devices;
    while (cur_dev) {
        printf("%S %S %s\n", cur_dev->manufacturer_string, cur_dev->product_string, cur_dev->path);

        char *bus = strtok(cur_dev->path, ":");
        char *dev = strtok(NULL, ":");
        char *iface = strtok(NULL, ":");
        if (!strcmp(iface, "02")) {
            busno = atoi(bus);
            devno = atoi(dev);
            printf("Found it.\n");
        }

        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devices);
    sprintf(path, "%04d:%04d:02", busno, devno);
    printf("%s\n", path);
    hid_device *hd = hid_open_path(path);
    if (!hd) {
        printf("Unable to open device.\n");
        return -1;
    }

    if (argv[1][0] == '0') {
        uint8_t report[7] = {
            0x10, 0x02, 0x0c, 0x1c, 0x00, 0x00, 0x00
        };
        hid_write(hd, report, 7);
    } else if (argv[1][0] == '1') {
        uint8_t report[7] = {
            0x10, 0x02, 0x0c, 0x1c, 0x01, 0x00, 0x00
        };
        hid_write(hd, report, 7);
    }

    hid_close(hd);

    FILE *fd = fopen("/sys/bus/usb/drivers/usbhid/bind", "w");
    fprintf(fd, "1-%d.%d:1.2", busno, devno);
    fclose(fd);
    
    return 0;
}
