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

bulk_ep_out = 0x03   # Out/Write EP is 0X03
bulk_ep_in = 0x83    # In/Read EP is 0x83

arduino_addr = 0x080E0000
wicedlib_addr = 0x08010000

reset_sec = 2


dfu_util = { 'Windows' : 'dfu-util.exe',
             'Darwin'  : 'dfu-util',
             'Linux'   : 'dfu-util' }

sdep_cmd_dict = { 'reboot' : 1, 'factory_reset' : 2, 'enter_dfu' : 3 }
######

def force_dfu_mode():
    # skip if already in DFU mode
    if usb.core.find(idVendor=usb_vid, idProduct=usb_dfu_pid) is None:
        runSystemCmd('enter_dfu')
        time.sleep(reset_sec)

# Send system command using control pipe
def runSystemCmd(cmd):
    # find our device
    dev = usb.core.find(idVendor=usb_vid, idProduct=usb_pid)
    if dev is None:
        print "Unable to connect to specified VID/PID 0x%04X 0x%04X" % (usb_vid, usb_pid)
        print "Try to connect in DFU mode"
        dev = usb.core.find(idVendor=usb_vid, idProduct=usb_dfu_pid)
        if dev is None:
            print "Unable to connect to specified VID/PID 0x%04X 0x%04X" % (usb_vid, usb_dfu_pid)
            sys.exit(1)
    dev.ctrl_transfer( 0x40, 0x5D, sdep_cmd_dict[cmd])

# HID OUT endpoint is 0x03, HID IN is 0x83
def sendSDEP(command):
    # send command to 0x03 Endpoint
    print ':'.join('{:02x}'.format(x) for x in command)
    dev.write(bulk_ep_out, command)

    # read response from 0x83 Endpoint
    resp = dev.read(bulk_ep_in, 64)
    print ':'.join('{:02x}'.format(x) for x in resp)

@click.group()
def cli():
    pass

@cli.command()
def reboot():
    runSystemCmd('reboot')

@cli.command()
def enter_dfu():
    force_dfu_mode()

@cli.command()
def factory_reset():
    runSystemCmd('factory_reset')

@cli.command()
@click.argument('binfile')
def firmware_upgrade(binfile):
    if not (binfile.endswith('.bin') and os.path.isfile(binfile)):
        click.echo('File not found or not a binary')
    else:
        time.sleep(reset_sec)
        force_dfu_mode()
        subprocess.call( ('dfu-util -d 0x%04X:0x%04X -a 0 -s 0x%08X:leave -D %s' % (usb_vid, usb_dfu_pid, wicedlib_addr, binfile)).split() )

@cli.command()
@click.argument('binfile')
def arduino_upgrade(binfile):
    if not (binfile.endswith('.bin') and os.path.isfile(binfile)):
        click.echo('File not found or not a binary')
    else:
        time.sleep(reset_sec)
        force_dfu_mode()
        subprocess.call( ('dfu-util -d 0x%04X:0x%04X -a 0 -s 0x%08X:leave -D %s' % (usb_vid, usb_dfu_pid, arduino_addr, binfile)).split() )

if __name__ == '__main__':
    cli()