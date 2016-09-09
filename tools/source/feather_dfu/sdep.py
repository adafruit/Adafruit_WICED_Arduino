import usb.core
import usb.util
import usb.control
import usb.backend.libusb0
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
SDEP_CMD_RESET               = 0x0001    # HW reset                                   
SDEP_CMD_FACTORYRESET        = 0x0002    # Factory reset                              
SDEP_CMD_DFU                 = 0x0003    # Enter DFU mode                             
SDEP_CMD_INFO                = 0x0004    # System information                         
SDEP_CMD_NVM_RESET           = 0x0005    # Reset DCT                                  
SDEP_CMD_ERROR_STRING        = 0x0006    # Get descriptive error string               
SDEP_CMD_COMMAND_STRING      = 0x0007    # Get descriptive SDEP command string
SDEP_CMD_SFLASH_ERASEALL     = 0x0008    # Erase SPI flash memory

# Hardware Command 
SDEP_CMD_GPIO                = 0x0100    # Set GPIO                                   
SDEP_CMD_RANDOMNUMBER        = 0x0101    # Random number

# DEBUG Commands 
SDEP_CMD_STACKDUMP           = 0x0300    # Dump the stack                             
SDEP_CMD_STACKSIZE           = 0x0301    # Get stack size                             
SDEP_CMD_HEAPDUMP            = 0x0302    # Dump the heap                              
SDEP_CMD_HEAPSIZE            = 0x0303    # Get heap size                              
SDEP_CMD_THREADLIST_PRINT    = 0x0304    # Get thread information                     

#******************************************************************************
# ERROR Constants
#******************************************************************************
ERROR_UNFINISHED         = 10

#******************************************************************************
# Configuration
#******************************************************************************
USB_VID = 0x239A
USB_PID = 0x0010
USB_PID_MSC = 0x8010
USB_DFU_PID = 0x0008

SDEP_DEBUG = 0
reset_sec = 2

# Explicitly configure the libusb backend for PyUSB to use libusb-1.0.
# On windows this will look for a libusb-1.0.dll in the same directory as the
# script/executable.
backend = None
if platform.system() == 'Windows':
    backend = usb.backend.libusb0.get_backend(find_library=lambda x: "libusb0_x86.dll")
else:
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
        if cmd_id > SDEP_CMD_SFLASH_ERASEALL:
            return

        self.msc_available = False

        # sys command will try to run system command with normal mode USB_VID
        # if device is not found, it will try to run with dfu mode USB_DFU_PID
        usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID, backend=backend)
        if usbdev is None:
            #try with MSC enabled PID
            usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID_MSC, backend=backend)
            if usbdev is None:
                usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_DFU_PID, backend=backend)
                if usbdev is None:
                    print "Unable to connect to feather board"
                    sys.exit(1)
            else:
                self.msc_available = True
        usbdev.ctrl_transfer( 0x40, SDEP_MSGTYPE_COMMAND, cmd_id)

        # in system command, only info has response data
        if cmd_id == SDEP_CMD_INFO:
            return self.get_response(cmd_id, usbdev)

    def execute(self, cmd_id, data=None):
        self.msc_available = False

        # send SDEP command via Control Transfer
        # find our device
        usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID)
        if usbdev is None:
            #try with MSC enabled PID
            usbdev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID_MSC, backend=backend)
            if usbdev is None:
                print "Unable to connect to feather board"
                sys.exit(1)
            else:
                self.msc_available = True

        # Send command phase
        if data is None:
            # no data payload
            usbdev.ctrl_transfer( 0x40, SDEP_MSGTYPE_COMMAND, cmd_id)
        else:
            if SDEP_DEBUG == 1:
                print ':'.join('{:02x}'.format(x) for x in data)

            if not isinstance(data[0], list):
                # single parameter
                usbdev.ctrl_transfer( 0x40, SDEP_MSGTYPE_COMMAND, cmd_id , 0, data)
            else:
                # multiple parameters
                combined_data = []
                for x in data:
                    # Len first then parameter
                    combined_data += [ ord(c) for c in struct.pack('<H', len(x)) ]
                    combined_data += x
                #print combined_data
                usbdev.ctrl_transfer( 0x40, SDEP_MSGTYPE_COMMAND, cmd_id , 0, combined_data)

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

