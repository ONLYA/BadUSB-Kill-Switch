# Software for BadUSB Kill Switch
This is a cross-platform software to run as a background service with a GUI to detect and prevent all possible HID attacks.
The structure of this is shown as the figure below.

![1][1]

Front-end has 3 keys to be achieved:
1. User Interface in either graphical or command-line way
2. Communicate with back-end worker service to:
* Start, stop and resume worker service
* Get alert of the unexpected HID devices plugged
3. Block all keyboard input if there is an alert received from the worker service

Back-end has 2 keys to be achieved:
1. Communicate with front-end to:
* Alert all unexpected HID devices plug-in
* Stop or resume itself according to the instructions from the front-end
2. Detect whether there is any HID devices other than those listed at the service start when device plug-in is detected


[1]:/resources/software_structure.png "The structure of the software"