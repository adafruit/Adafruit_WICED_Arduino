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
from sdep import *

# add /usr/local/bin to PATH, required for OSX to execute dfu-util
os.environ["PATH"] = os.environ["PATH"] + ':/usr/local/bin'

###### CONFIGURATION
arduino_addr = 0x080E0000
featherlib_addr = 0x08010000

dfu_util = { 'Windows' : os.path.dirname(sys.argv[0]) + '/windows/dfu-util/dfu-util.exe',
             'Darwin'  : 'dfu-util',
             'Linux'   : 'dfu-util' }

@click.group()
def cli():
    pass

@cli.command()
def reboot():
    sdep.reboot()

@cli.command()
def enter_dfu():
    sdep.enter_dfu()

@cli.command()
def factory_reset():
    sdep.syscommand(SDEP_CMD_FACTORYRESET)

@cli.command()
def nvm_reset():
    sdep.syscommand(SDEP_CMD_NVM_RESET)

@cli.command()
def info():
    resp = sdep.syscommand(SDEP_CMD_INFO)
    click.echo(resp.tostring())

@cli.command()
@click.argument('binfile')
def featherlib_upgrade(binfile):
    if not (binfile.endswith('.bin') and os.path.isfile(binfile)):
        click.echo('File not found or not a binary')
    else:
        time.sleep(reset_sec)
        sdep.enter_dfu()
        subprocess.call( ('%s -d 0x%04X:0x%04X -a 0 -s 0x%08X:leave -D %s' % (dfu_util[platform.system()], USB_VID, USB_DFU_PID, featherlib_addr, binfile)).split() )

@cli.command()
@click.argument('binfile')
def arduino_upgrade(binfile):
    if not (binfile.endswith('.bin') and os.path.isfile(binfile)):
        click.echo('File not found or not a binary')
    else:
        time.sleep(reset_sec)
        sdep.enter_dfu()
        subprocess.call( ('%s -d 0x%04X:0x%04X -a 0 -s 0x%08X:leave -D %s' % (dfu_util[platform.system()], USB_VID, USB_DFU_PID, arduino_addr, binfile)).split() )

if __name__ == '__main__':
    cli()
