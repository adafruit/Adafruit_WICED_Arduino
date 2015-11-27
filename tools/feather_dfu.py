import usb.backend.libusb1
import usb.core
import usb.util
import usb.control
import click
import platform
import os
import subprocess
import sys
import time

# add /usr/local/bin to PATH, required for OSX to execute dfu-util
os.environ["PATH"] = os.environ["PATH"] + ':/usr/local/bin'

###### CONFIGURATION

usb_vid = 0x239A
usb_pid = 0x0009
usb_dfu_pid = 0x0008

arduino_addr = 0x080E0000
featherlib_addr = 0x08010000

SDEP_MSGTYPE_COMMAND = 0x10
SDEP_MSGTYPE_RESPONSE = 0x20
SDEP_MSGTYPE_ALERT = 0x40
SDEP_MSGTYPE_ERROR = 0x80

SDEP_CMD_RESET          = 0x0001
SDEP_CMD_FACTORYRESET   = 0x0002
SDEP_CMD_DFU            = 0x0003

reset_sec = 2

dfu_util = { 'Windows' : os.path.dirname(sys.argv[0]) + '/windows/dfu-util/dfu-util.exe',
             'Darwin'  : 'dfu-util',
             'Linux'   : 'dfu-util' }

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

# Send system command using control pipe
def runSystemCmd(cmd_id):
    # find our device
    dev = usb.core.find(idVendor=usb_vid, idProduct=usb_pid, backend=backend)
    if dev is None:
        print "Unable to connect to specified VID/PID 0x%04X 0x%04X" % (usb_vid, usb_pid)
        print "Try to connect in DFU mode"
        dev = usb.core.find(idVendor=usb_vid, idProduct=usb_dfu_pid, backend=backend)
        if dev is None:
            print "Unable to connect to specified VID/PID 0x%04X 0x%04X" % (usb_vid, usb_dfu_pid)
            sys.exit(1)
    dev.ctrl_transfer( 0x40, SDEP_MSGTYPE_COMMAND, cmd_id)

def force_dfu_mode():
    # skip if already in DFU mode
    if usb.core.find(idVendor=usb_vid, idProduct=usb_dfu_pid, backend=backend) is None:
        runSystemCmd(SDEP_CMD_DFU)
        time.sleep(reset_sec)

@click.group()
def cli():
    pass

@cli.command()
def reboot():
    runSystemCmd(SDEP_CMD_RESET)

@cli.command()
def enter_dfu():
    force_dfu_mode()

@cli.command()
def factory_reset():
    runSystemCmd(SDEP_CMD_FACTORYRESET)

@cli.command()
@click.argument('binfile')
def featherlib_upgrade(binfile):
    if not (binfile.endswith('.bin') and os.path.isfile(binfile)):
        click.echo('File not found or not a binary')
    else:
        time.sleep(reset_sec)
        force_dfu_mode()
        subprocess.call( ('%s -d 0x%04X:0x%04X -a 0 -s 0x%08X:leave -D %s' % (dfu_util[platform.system()], usb_vid, usb_dfu_pid, featherlib_addr, binfile)).split() )

@cli.command()
@click.argument('binfile')
def arduino_upgrade(binfile):
    if not (binfile.endswith('.bin') and os.path.isfile(binfile)):
        click.echo('File not found or not a binary')
    else:
        time.sleep(reset_sec)
        force_dfu_mode()
        subprocess.call( ('%s -d 0x%04X:0x%04X -a 0 -s 0x%08X:leave -D %s' % (dfu_util[platform.system()], usb_vid, usb_dfu_pid, arduino_addr, binfile)).split() )

if __name__ == '__main__':
    cli()
