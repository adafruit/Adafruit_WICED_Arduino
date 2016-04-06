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
SDEP_CMD_RESET               = 0x0001    # HW reset                                   
SDEP_CMD_FACTORYRESET        = 0x0002    # Factory reset                              
SDEP_CMD_DFU                 = 0x0003    # Enter DFU mode                             
SDEP_CMD_INFO                = 0x0004    # System information                         
SDEP_CMD_NVM_RESET           = 0x0005    # Reset DCT                                  
SDEP_CMD_ERROR_STRING        = 0x0006    # Get descriptive error string               

# Hardware Command 
SDEP_CMD_GPIO                = 0x0100    # Set GPIO                                   
SDEP_CMD_RANDOMNUMBER        = 0x0101    # Random number                              

# SPI Flash Commands 
SDEP_CMD_SFLASHFORMAT        = 0x0200    # Format SPI flash memory                    
SDEP_CMD_SFLASHLIST          = 0x0201    # List SPI flash contents                    

# DEBUG Commands 
SDEP_CMD_STACKDUMP           = 0x0300    # Dump the stack                             
SDEP_CMD_STACKSIZE           = 0x0301    # Get stack size                             
SDEP_CMD_HEAPDUMP            = 0x0302    # Dump the heap                              
SDEP_CMD_HEAPSIZE            = 0x0303    # Get heap size                              
SDEP_CMD_THREADLIST          = 0x0304    # Get thread information                     

# WiFi Commands 
SDEP_CMD_SCAN                = 0x0400    # AP scan                                    
SDEP_CMD_CONNECT             = 0x0401    # Connect to AP                              
SDEP_CMD_DISCONNECT          = 0x0402    # Disconnect from AP                         
SDEP_CMD_APSTART             = 0x0403    # Start AP                                   
SDEP_CMD_APSTOP              = 0x0404    # Stop AP                                    
SDEP_CMD_WIFI_GET_RSSI       = 0x0405    # Get RSSI of current connected signal       
SDEP_CMD_WIFI_PROFILE_ADD    = 0x0406    # Add a network profile                      
SDEP_CMD_WIFI_PROFILE_DEL    = 0x0407    # Remove a network profile                   
SDEP_CMD_WIFI_PROFILE_CLEAR  = 0x0408    # Clear all network profiles                 
SDEP_CMD_WIFI_PROFILE_CHECK  = 0x0409    # Check if a network profile exists          
SDEP_CMD_WIFI_PROFILE_SAVE   = 0x040A    # Save current connected profile to NVM      
SDEP_CMD_WIFI_PROFILE_GET    = 0x040B    # Get AP's profile info                      
SDEP_CMD_TLS_SET_ROOT_CERTS  = 0x040C    # Set ROOT CA Chain                          

# Gateway Commands 
SDEP_CMD_GET_IPV4_ADDRESS    = 0x0500    # Get IPv4 address from an interface         
SDEP_CMD_GET_IPV6_ADDRESS    = 0x0501    # Get IPv6 address from an interface         
SDEP_CMD_GET_GATEWAY_ADDRESS = 0x0502    # Get IPv6 gateway address                   
SDEP_CMD_GET_NETMASK         = 0x0503    # Get IPv4 DNS netmask                       
SDEP_CMD_GET_MAC_ADDRESS     = 0x0504    # Get MAC Address                            

# Network Commands 
SDEP_CMD_PING                = 0x0600    # Ping                                       
SDEP_CMD_DNSLOOKUP           = 0x0601    # DNS lookup                                 
SDEP_CMD_GET_ISO8601_TIME    = 0x0602    # Get time                                   
SDEP_CMD_GET_UTC_TIME        = 0x0603    # Get UTC time in seconds                    

# TCP Commands 
SDEP_CMD_TCP_CONNECT         = 0x0700    # Create TCP stream socket and connect       
SDEP_CMD_TCP_WRITE           = 0x0701    # Write to the TCP stream socket             
SDEP_CMD_TCP_FLUSH           = 0x0702    # Flush TCP stream socket                    
SDEP_CMD_TCP_READ            = 0x0703    # Read from the TCP stream socket            
SDEP_CMD_TCP_DISCONNECT      = 0x0704    # Disconnect TCP stream socket               
SDEP_CMD_TCP_AVAILABLE       = 0x0705    # Check if there is data in TCP stream socket
SDEP_CMD_TCP_PEEK            = 0x0706    # Peek at byte data from TCP stream socket   
SDEP_CMD_TCP_STATUS          = 0x0707    # Get status of TCP stream socket            
SDEP_CMD_TCP_SET_CALLBACK    = 0x0708    # Set callback function for TCP connection   

# UDP Commands 
SDEP_CMD_UDP_CREATE          = 0x0800    # Create UDP socket                          
SDEP_CMD_UDP_WRITE           = 0x0801    # Write to the UDP socket                    
SDEP_CMD_UDP_FLUSH           = 0x0802    # Flush UDP stream socket                    
SDEP_CMD_UDP_READ            = 0x0803    # Read from the UDP stream socket            
SDEP_CMD_UDP_CLOSE           = 0x0804    # Close UDP stream socket                    
SDEP_CMD_UDP_AVAILABLE       = 0x0805    # Check if there is data in UDP stream socket
SDEP_CMD_UDP_PEEK            = 0x0806    # Peek at byte data from UDP stream socket   
SDEP_CMD_UDP_PACKET_INFO     = 0x0807    # Get packet info of UDP stream socket       

# MQTT Commands 
SDEP_CMD_MQTTCONNECT         = 0x0900    # Connect to a broker                        
SDEP_CMD_MQTTDISCONNECT      = 0x0901    # Disconnect from a broker                   
SDEP_CMD_MQTTPUBLISH         = 0x0902    # Publish a message to a topic               
SDEP_CMD_MQTTSUBSCRIBE       = 0x0903    # Subscribe to a topic                       
SDEP_CMD_MQTTUNSUBSCRIBE     = 0x0904    # Unsubscribe from a topic                   
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

# Explicitly configure the libusb backend for PyUSB.
backend = None
if platform.system() == 'Windows':
    # On Windows give a path to the included libusb-1.0.dll so users don't
    # need to have it installed.
    libusb_dll = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                              'dfu-util', 'windows', 'libusb-1.0.dll')
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