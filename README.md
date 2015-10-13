# Adafruit WICED Arduino

This private repo contains files used for the WICED/STM32F205 and Arduino IDE integration work.  

These files will eventually end up in the /hardware folder of the Arduino IDE.

## Preparing the Hardware

This library will require that the WICED firmware and USB DFU bootloader has already been flashed to the device via the [Adafruit_BroadcomWiFi_Core](https://github.com/adafruit/Adafruit_BroadcomWiFi_Core) repo.

From the **development** branch, run `make clean all flash-all` in the `projects/adafruittest` folder with a Segger J-Link connected.  Be sure to connect VTRef to 3.3V on the WICED development board in addition to the SWDIO, SWCLK and Reset pins.

## Arduino Setup

- Create a **hardware** folder in `~/Documents/Arduino` (OS X) or `My Documents\Arduino` (Windows) if it doesn't already exist
- Clone this repo to the root of the hardware folder, or download as a .zip and unzip it in `hardware/Adafruit_WICED_Arduino`

	```
	git clone git@github.com:adafruit/Adafruit_WICED_Arduino.git
	```

- Install the necessary GCC toolchain for ARM: Tools->Board->Board Manager --> Download **Arduino SAM Boards (32-bits ARM Cortex-M3)**
- Restart the Arduino IDE
- Install [dfu-util](http://dfu-util.sourceforge.net/)

	On OS X you can install dfu-util via:
	```
	brew install dfu-util
	```
	On Windows dfu-util.exe is already added in the `tools/windows/dfu-util`. To install driver for bootloader, you can use [Zadig](http://zadig.akeo.ie/) to map the DFU device to libusb or choose windows driver in `drivers/windows` 
	
- Install Python 2.7, python-pip, pyusb, click (we use a python script to flash the boards for now)

	```
	pip install --pre pyusb
	pip install click
	```

- Install [AdaLink](https://github.com/adafruit/Adafruit_Adalink), which is used to flash the bootloader

## Burnning bootloader

This requires either jlink or stlink v2 as programmer.

- In the "Tools > Board" menu select **Feather WICED** as the hardware target
- In "Tools > Programmer", select either **Jlink with Adalink** or **Stlink v2 with Adalink**

then select "Tools > Burn bootloader", this will also perform a whole chip erased (which does take some time).

## Flashing featherlib

After burning bootloader, you need to flash featherlib which contain all Adafruit wifi library. It resides on a different flash bank than Arduino sketch and only need to be flashed once.

- Choose "Tools > Section > Featherlib"
- Click on upload icon to start uploading featherlib/featherlib.bin (Arduino will try to compile the current sketch, but it is really irrelevant)

It will take awhile to upload featherlib due to its massive size.

## Building a Project

- Click the "File > Open ..." menu item and load the **blink.ino** sketch in the hardware folder created above (`Adafruit_WICED_Arduino/examples/Digital/Blink/Blink.ino`)
- Build then flash the sketch as normal. You don't need to select a specific debugger since USB-DFU should run via a Python script:

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
