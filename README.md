# Adafruit WICED Feather Arduino BSP

This repository contains files used for the Adafruit WICED Feather WiFi board,
and allows the WICED Feather to be used within the Arduino IDE.

These files should be placed in the `hardware` folder of the Arduino IDE, as
detailed below:

## Arduino Setup

- Create a **hardware** folder in `~/Documents/Arduino` (OS X) or
  `My Documents\Arduino` (Windows) if it doesn't already exist
- Clone this repo to the root of the hardware folder, or download as a .zip and
  unzip it in `hardware/Adafruit_WICED_Arduino`

	```
	git clone https://github.com/adafruit/Adafruit_WICED_Arduino.git
	```

- Install the necessary GCC toolchain for ARM: Tools->Board->Board Manager --> Download **Arduino SAM Boards (32-bits ARM Cortex-M3)**
- Restart the Arduino IDE
- OSX or Linux only: Install [dfu-util](http://dfu-util.sourceforge.net/)

	On **OS X** you can install dfu-util via:
	```
	brew install dfu-util
	```
	On **Windows** dfu-util.exe is included and will be used automatically.

	On **Linux (Debian or Ubuntu)** you can install dfu-util from the apt package manager with `sudo apt-get install dfu-util`.

  To install the drivers for the bootloader on Windows, you can use the .inf
  files in in `drivers/windows`.

- On **OSX or Linux** install Python 2.7, python-pip, pyusb, click (we use `tools/feather_dfu.py`
  to flash the device over USB DFU from the Arduino IDE):
	```
	pip install --pre pyusb
	pip install click
	```

	On Windows the programming script is bundled with Python and includes all
	dependencies automatically.

- Install [AdaLink](https://github.com/adafruit/Adafruit_Adalink), which is
  used to flash the bootloader (optional)

- On Linux you will need to add a small udev rule to make the WICED board available to non-root users.  If you don't have
  this rule then you'll see permission errors from the Arduino IDE when it attempts to program the board.  Create or edit a   file called /etc/udev/rules.d/99-adafruit-boards.rules and add the following lines:

    SUBSYSTEM=="usb", ATTR{idProduct}=="0010", ATTRS{idVendor}=="239a", MODE="0660", GROUP="dialout"
	SUBSYSTEM=="usb", ATTR{idProduct}=="8010", ATTRS{idVendor}=="239a", MODE="0660", GROUP="dialout"
    SUBSYSTEM=="usb", ATTR{idProduct}=="0008", ATTRS{idVendor}=="239a", MODE="0660", GROUP="dialout"

  Depending on your distribution you might need to change `GROUP="dialout"` to a different value like `"users"`.  The
  dialout group should work for Ubuntu.

  Then restart udev with:

        sudo restart udev

  Or on systemd-based systems like the latest Debian or Ubuntu 15.04+ restart udev with:

        sudo systemctl restart udev

## Flashing the Bootloader (Optional)

This library requires the USB DFU bootloader to have previously been flashed
to the device.

All devices ship from Adafruit with the bootloader in place and you should
almost never have to worry about flashing this yourself, but if necessary you
can reflash the bootloader using a Segger J-Link or an ST ST-Link/V2:

- In the **Tools > Board** menu select **Adafruit WICED Feather**
- Select the appropriate debugger in **Tools > Programmer** (ex.: `JLink with AdaLink`)
- Connect the debug probe to the SWD pins on the modules (including setting
  the VTRef pin to 3.3V if you are using a Segger J-Link)
- Select **Burn Bootloader**

Note that this will also perform a full flash erase, which may take some time.

For internal development (if you have access to the NDA restricted [Adafruit_BroadcomWiFi_Core](https://github.com/adafruit/Adafruit_BroadcomWiFi_Core)
repo), go to the **development** branch, and run `make clean all flash-all` in
the `projects/adafruittest` folder with a Segger J-Link connected.  Be sure to
connect VTRef to 3.3V on the WICED development board in addition to the SWDIO,
SWCLK and Reset pins

## Flashing featherlib

Once the bootloader is in place, you need to flash featherlib which contains
the Adafruit WiFi library. It resides on a different flash bank than the Arduino
sketch and only needs to be flashed once, unless an update is available at
which point it should be flashed again.

- Choose **Tools > Section > Featherlib**
- Click the upload icon to start uploading `featherlib/featherlib.bin`
  (Arduino will try to compile the current sketch, but it is irrelevant)

It will take a moment to upload featherlib due to the file size.

## Building a Project

- Click the **File > Open ...** menu item and load your sketch
- Make sure the Section is set to `User Code` in **Tools > Section**
- Build then flash the sketch as normal. You don't need to select a specific
  debugger since USB-DFU should run via a Python script:

```
Sketch uses 15,148 bytes (1%) of program storage space. Maximum is 1,048,576 bytes.
Global variables use 12,952 bytes of dynamic memory.
dfu-util 0.8

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2014 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to dfu-util@lists.gnumonks.org

dfu-util: Invalid DFU suffix signature
dfu-util: A valid DFU suffix will be required in a future dfu-util release!!!
Opening DFU capable USB device...
ID 239a:0008
Run-time device DFU version 011a
Claiming USB DFU Interface...
Setting Alternate Setting #0 ...
Determining device status: state = dfuIDLE, status = 0
dfuIDLE, continuing
DFU mode device DFU version 011a
Device returned transfer size 1024
DfuSe interface name: "Internal Flash   "
Downloading to address = 0x080e0000, size = 22020

Download	[                         ]   0%            0 bytes
Download	[                         ]   0%            0 bytes
Download	[=                        ]   4%         1024 bytes
Download	[==                       ]   9%         2048 bytes
Download	[===                      ]  13%         3072 bytes
Download	[====                     ]  18%         4096 bytes
Download	[=====                    ]  23%         5120 bytes
Download	[======                   ]  27%         6144 bytes
Download	[========                 ]  32%         7168 bytes
Download	[=========                ]  37%         8192 bytes
Download	[==========               ]  41%         9216 bytes
Download	[===========              ]  46%        10240 bytes
Download	[============             ]  51%        11264 bytes
Download	[=============            ]  55%        12288 bytes
Download	[===============          ]  60%        13312 bytes
Download	[================         ]  65%        14336 bytes
Download	[=================        ]  69%        15360 bytes
Download	[==================       ]  74%        16384 bytes
Download	[===================      ]  79%        17408 bytes
Download	[====================     ]  83%        18432 bytes
Download	[======================   ]  88%        19456 bytes
Download	[=======================  ]  93%        20480 bytes
Download	[======================== ]  97%        21504 bytes
Download	[=========================] 100%        22020 bytes
Download done.
File downloaded successfully
dfu-util: Error during download get_status
```

Note: The error at the end can be ignored, and is related to a problem with the dfu utility.

## Updating Windows Binaries

The script to upload to the board is written in Python and located in the
`tools/source/feather_dfu` folder.  For Windows users a pre-built binary version
is included and used by the Arduino IDE so that Python and other dependencies
do not need to be installed.  However this means any time the feather_dfu script
is modified a new Windows binary will need to be generated.  Follow the steps below
on a Windows machine with the **32-bit version** of Python (for maximum compatibility
with all Windows versions, since 32-bit executables can run on 64-bit Windows too).

First you need to install the dependencies of the script.  This is only done
once (unless the dependencies change).  From inside the `tools/source/feather_dfu`
folder run:
```
pip install -r requirements.txt --pre
```

Now to generate a new executable and its related files use the setup.py script
and its `build_exe` action.  Internally this is using the cx_Freeze module to
embed the script with a Python interpretor and all its depenedencies.
```
python setup.py build_exe
```

The output of this command will be in the `build` subdirectory.  Inside will be
a subdirectory for the current platform (like `exe.win32-2.7`), and inside of that
folder will be all of the necessary files for the feather_dfu binary.  Copy all
of these files over the files in the `tools/win32-x86/feather_dfu` folder to
update the binaries.

Note that inside `tools/win32-x86/feather_dfu` there will be one extra file that
is not included in the build steps above, libusb-1.0.dll.  This is the 32-bit
version of libusb-1.0.dll which can be downloaded from http://libusb.info.  The
DLL **must** be located inside the feather_dfu directory next to the executable.

## Windows 32-bit Python side-by-side 64-bit Python

Typically on Windows you will install the 64-bit version of Python, however
this presents a challenge when building binaries with cx_Freeze since it will
use this 64-bit version of Python exclusively.  To generate a 32-bit binary you
will need to install a 32-bit version of Python side-by-side the 64-bit version.
Unfortunately this is a tricky process but it can be simplified using the
virtualenv tool.

The steps below outline a general process for installing 64-bit Python as the
main Python interpretor and using a 32-bit Python virtualenv side-by-side to
build 32-bit executables with cx_Freeze.

First install Python 2.7 **x64** in its normal C:\Python27 location.

Next install Python 2.7 **x86** but override the location to be something unique
like C:\Python27-x86.  Also you **must** set the register extensions option in the
installer to disabled/not installed (this will prevent the 32-bit version taking
over the default path, etc.).

Now open a command window and navigate to the path that Python 32bit was installed
and run the `scripts/pip.exe` to install virtualenv in 32bit python:
```
c:\Python27-x86\Scripts\pip.exe install virtualenv
```

To create a virtualenv use the virtualenv.exe (also installed in the Scripts folder)
like normal.  For example to create a new virtual environment called py32 open a
terminal in your home directory and run:
```
c:\Python27-x86\Scripts\virtualenv.exe py32
```

The created virtualenv will have a Scripts subdirectory with an activate.bat.  When
this bat file is run it will setup the environment to use Python 32-bit.  For example
from your home directory run in a command terminal:
```
.\py32\Scripts\activate.bat
```

Now Python, pip, etc. should all be using the 32-bit Python virtual environment.
You can build software with cx_Freeze etc and it will use this 32-bit version
of Python.

## Frequently Asked Questions

### Q1

**Q:** When I try to build I'm getting `Cannot run program "{runtime.tools.arm-none-eabi-gcc.path}\bin\arm-none-eabi-g++" (in directory "."): CreateProcess error=2, The system cannot find the file specified`?

**A:** This is probably because you don't have the ARM Cortex M3 toolchain installed.  Install the necessary GCC toolchain for ARM from the Arduino Board Manager via: **Tools->Board->Board Manager** then download **Arduino SAM Boards (32-bits ARM Cortex-M3)**

### Q2

**Q:** When I try to flash using USB DFU I get the following error from feather_dfu.py: `Traceback (most recent call last): File "...\hardware\Adafruit_WICED_Arduino/tools/feather_dfu.py", line 1, in <module> import usb.backend.libusb1`?

**A:** This is probably caused by an old version of pysub.  Update your pyusb version to 1.0b or higher via the following command:

```
pip install --upgrade pyusb
```

Also make sure that you have installed the proper Windows driver .inf files so that libusb can access the device via the correct USB VID and PID combinations.
