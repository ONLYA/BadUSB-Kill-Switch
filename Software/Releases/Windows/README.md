This is the HID Kill Switch software for Windows 10.

## Installation
Firstly, make sure you have Python 3.7+ installed.

Secondly, install packages by directly running `install.bat`.

After all the packages are installed, you are good to go. Run `BadUSB Kill Switch.exe` for the protection!

## Usage
After the first notification is poped as "Start up", the application is minimised in the system tray. Right click it to select "Switch Service" to turn it on. *(Please note that the service will check based on the connected devices at the beginning of the service.)*

If an unexpected HID device is detected, the warning will pop up with both a notification and a warning box. Now all the keyboard input are blocked. To unblock the input, click the button on the warning box. Service will restart after the notice.

To stop the service temporarily, click the checked "Switch Service" in the context menu.

To quit the GUI and the service, click "Quit Program" in the context menu.
