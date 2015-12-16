import usb.core
import usb.util
import usb.control
import usb.backend.libusb1
import time
import os
import sys
import platform

#******************************************************************************
# SDEP Command Constant
#******************************************************************************
# Command Type
SDEP_MSGTYPE_COMMAND     = 0x10
SDEP_MSGTYPE_RESPONSE    = 0x20
SDEP_MSGTYPE_ALERT       = 0x40
SDEP_MSGTYPE_ERROR       = 0x80

# General Purpose
SDEP_CMD_RESET          = 0x0001
SDEP_CMD_FACTORYRESET   = 0x0002
SDEP_CMD_DFU            = 0x0003
SDEP_CMD_INFO           = 0x0004
SDEP_CMD_NVM_RESET      = 0x0005

# GPIO Command
SDEP_CMD_GPIO           = 0x0010
SDEP_CMD_RANDOMNUMBER   = 0x0011

# WiFi Commands
SDEP_CMD_SCAN           = 0x0020
SDEP_CMD_CONNECT        = 0x0021
SDEP_CMD_DISCONNECT     = 0x0022
SDEP_CMD_APSTART        = 0x0023
SDEP_CMD_APSTOP         = 0x0024

# Network Commands
SDEP_CMD_PING           = 0x0030
SDEP_CMD_DNSLOOKUP      = 0x0031
SDEP_CMD_GETTIME        = 0x0032
SDEP_CMD_HTTPGETURI     = 0x0033
SDEP_CMD_HTTPPOST       = 0x0034
SDEP_CMD_HTTPSGET       = 0x0035
SDEP_CMD_HTTPREQUEST    = 0x0036
SDEP_CMD_HTTPSREQUEST   = 0x0037

# DEBUG Commands
SDEP_CMD_STACKDUMP      = 0x0040
SDEP_CMD_STACKSIZE      = 0x0041
SDEP_CMD_HEAPDUMP       = 0x0042
SDEP_CMD_HEAPSIZE       = 0x0043
SDEP_CMD_THREADLIST     = 0x0044

# SPI Flash Commands
SDEP_CMD_SFLASHFORMAT   = 0x0050
SDEP_CMD_SFLASHLIST     = 0x0051

# MQTT Commands
SDEP_CMD_MQTTLASTWILL   = 0x0060
SDEP_CMD_MQTTCONNECT    = 0x0061
SDEP_CMD_MQTTDISCONNECT = 0x0062
SDEP_CMD_MQTTPUBLISH    = 0x0063
SDEP_CMD_MQTTSUBSCRIBE  = 0x0064
SDEP_CMD_MQTTUNSUBSCRIBE= 0x0065

# IRQ Commands
SDEP_CMD_IRQREAD        = 0x0070
SDEP_CMD_IRQCOUNT       = 0x0071
SDEP_CMD_IRQAVAIL       = 0x0072
SDEP_CMD_IRQCLEAR       = 0x0073

ERROR_UNFINISHED         = 10

#******************************************************************************
# Configuration
#******************************************************************************
USB_VID = 0x239A
USB_PID = 0x0009
USB_PID_MSC = 0x8009
USB_DFU_PID = 0x0008

SDEP_DEBUG = 0
reset_sec = 2

# Explicitly configure the libusb backend for PyUSB.
backend = None
if platform.system() == 'Windows':
    # On Windows give a path to the included libusb-1.0.dll so users don't
    # need to have it installed.
    libusb_dll = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                              'windows', 'dfu-util', 'libusb-1.0.dll')
    usb.backend.libusb1.get_backend(find_library=lambda x: libusb_dll)
else:
    # On other platforms libusb-1.0 needs to be installed.
    backend = usb.backend.libusb1.get_backend()

class Sdep(object):
    def enter_dfu(self):
        # skip if already in DFU mode
        if usb.core.find(idVendor=USB_VID, idProduct=USB_DFU_PID, backend=backend) is None:
            self.syscommand(SDEP_CMD_DFU)
            time.sleep(reset_sec)

    def reboot(self):
        self.syscommand(SDEP_CMD_RESET)

    def syscommand(self, cmd_id):
        if cmd_id > SDEP_CMD_NVM_RESET:
            return

        # sys command will try to run system command with normal mode USB_VID = 0x0009
        # if device is not found, it will try to run with dfu mode USB_VID = 0x0008
        usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID, backend=backend)
        if usbdev is None:
            #try with MSC enabled PID
            usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID_MSC, backend=backend)
            if usbdev is None:
                usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_DFU_PID, backend=backend)
                if usbdev is None:
                    print "Unable to connect to feather board"
                    sys.exit(1)
        usbdev.ctrl_transfer( 0x40, SDEP_MSGTYPE_COMMAND, cmd_id)

        # in system command, only info has response data
        if cmd_id == SDEP_CMD_INFO:
            return self.get_response(cmd_id, usbdev)

    def execute(self, cmd_id, data=None):
        # send SDEP command via Control Transfer
        # find our device
        usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID)
        if usbdev is None:
            #try with MSC enabled PID
            usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID_MSC, backend=backend)
            if usbdev is None:
                print "Unable to connect to feather board"
                sys.exit(1)

        # Send command phase
        # Uncomment to print out command bytes
        if SDEP_DEBUG == 1:
            print ':'.join('{:02x}'.format(x) for x in data)

        if data is None:
            usbdev.ctrl_transfer( 0x40, SDEP_MSGTYPE_COMMAND, cmd_id)
        else:
            usbdev.ctrl_transfer( 0x40, SDEP_MSGTYPE_COMMAND, cmd_id , 0, data)
        time.sleep(0.1)

        return self.get_response(cmd_id, usbdev)

    def get_response(self, cmd_id, usbdev):
        # Response phase
        while True:
            resp_content = usbdev.ctrl_transfer( 0xC0, SDEP_MSGTYPE_RESPONSE, cmd_id, 0, 4096)
            if resp_content[0] == SDEP_MSGTYPE_ERROR and ((resp_content[2] << 8) + resp_content[1]) == ERROR_UNFINISHED:
                time.sleep(1)
            else:
                break;

        # Uncomment to print out response bytes
        if SDEP_DEBUG == 1:
            print ':'.join('{:02x}'.format(x) for x in resp_content)

        # skip header
        return resp_content[4:]

sdep = Sdep()