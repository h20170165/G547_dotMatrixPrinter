# G547_dotMatrixPrinter
Project submission for Device Drivers course.

### Summary
This project aims to demonstrate the functionality of a custom dot-matrix printer, that is interfaced via USB. The hardware part includes stepper & servo motors, ATMEGA8 microcontroller, L293D motor drivers,components for interfacing to USB and a mock printer model. The firmware part includes a C program that controls PORTB & PORTC output depending on the USB request received from the USB core. The device driver obtains user-space requests over IOCTL and submits those requests to USB core via usb_control_msg(). The driver is automatically plugged in (registered) on detection of an allowed device (allowed devices are controlled via id-table) and unregistered on disconnection. The user-space program (uprinter_usr.c) first stores a raw bitmap image into a linear array, and then transfers this data over ioctl requests in a printing-algorithm for 82x82 bitmaps.

### Hardware Schematic
![Schematic](https://github.com/h20170165/G547_dotMatrixPrinter/blob/master/dd_schem.png)

### Hardware Snapshots
![Snapshot1](https://github.com/h20170165/G547_dotMatrixPrinter/blob/master/P_20180427_131356.jpg)
![Snapshot2](https://github.com/h20170165/G547_dotMatrixPrinter/blob/master/P_20180427_131451.jpg)

### Firmware Description
Firmware is present in the folder /firmware. Relevant files are main.c, Makefile, and ../commands.h. Folder usbdrv and file usbconfig.h are also necessery for successful build. This firmware closely follows the V-USB implementation by Starkjohann. "make hex" builds main.hex file and "make flash" flashes hex file to the hardware but requires hardware to be reset once to enter USBasp mode.
* **Build process**:
```shell
$ make clean
$ make hex
$ make flash
```

### Driver Description
Device Driver is present in the root directory. Relevant files are uprinter.c, Makefile, and commands.h. Device driver is a custom-class character driver implemented over USB interface. "make kern" or "make all" is used to build .o and .ko files. "make insert" is used to insert the kernel-module, and "make remove" can be used to remove the kernel-module. "sudo rmmod usbhid" is required for proper functionality of the custom driver.
* **Build process**:
```shell
$ sudo rmmod usbhid
$ make clean
$ make kern
$ make insert
```

### User Space Application
User-space applications are present in the root directory. Relevant files are uprinter_usr.c, Makefile, commands.h, test.txt, test2.txt & test3.txt. The text files are random raw bitmap files used for printing testing purpose. "make user" is used to build the primary user-space program whereas "make diag" is used to build two diagnosstic programs used for images & motors testing. "make run" is used to run the primary user-space application. Diagnostic programs are not explained here and there use is self-explanatory from the source files "updiag.c" and "picdiag.c".
* **Build process**:
```shell
$ make user
$ make diag
$ make run
```

### Authors
Sarthi Chugh & Vipin Kumar Verma, ME Embedded Systems, BITS Pilani 

## License
* This project is licensed under the GNU-GPL License.
