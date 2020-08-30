# Hardware for BadUSB Kill Switch

This uses ATSAMD21E17D as the main chip for detection. This device is powered via the USB *Male* port to computer. When a USB device is plugged into the USB *Female* port on this device, it will start to detect whether the USB device is HID device or not.
If the inserted USB device is NOT recognised as an HID, the **green** light will be lit and the data paths of it are switched on.
If the inserted USB device is recognised as an HID, the **red** light will be lit while the **green** light is off and the datapaths of it will be switched off.

This device will keep detecting the USB protocol during the whole insertion in order to prevent possible remote or delayed attacks.

## Electronics [(Folder)](/Hardware/Electronics)
It contains the PCB and circuit design files of this device including the BOM file in csv format. Now it is completed.

## firmware [(Folder)](/Hardware/firmware)
It contains the firmware of the device so that it will work as described above. Now it is NOT completed.
