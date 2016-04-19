import click
import platform
import os
import subprocess
import sys
import time
from sdep import *

# add /usr/local/bin to PATH, required for OSX to execute dfu-util
if platform.system() == 'Darwin':
    os.environ["PATH"] = os.environ["PATH"] + ':/usr/local/bin'

###### CONFIGURATION
arduino_addr = 0x080E0000
featherlib_addr = 0x08010000

# Global state:
dfu_util_path = None


# Use dfu_util to download a .bin file to the hardware.
# Must specify the path to dfu_util, the path to a .bin file to upload, and the
# address to place the .bin in memory on the device.
def dfu_download_bin(dfu_util, binfile, address):
    if not binfile.lower().endswith('.bin'):
        raise click.ClickException('Expected file to be a .bin!')
    time.sleep(reset_sec)
    sdep.enter_dfu()
    subprocess.call([dfu_util,
                    '-d', '0x{0:04X}:{1:04X}'.format(USB_VID, USB_DFU_PID),
                    '-a', '0',
                    '-s', '0x{0:08X}:leave'.format(address),
                    '-D', binfile])


@click.group()
@click.option('--dfu-util', '-u',
              default='dfu-util',
              help='Path to dfu-util binary.  If not specified dfu-util must be in the path.')
def cli(dfu_util):
    global dfu_util_path
    dfu_util_path = dfu_util

@cli.command()
def reboot():
    """Perform a reboot/reset (ATZ) of the board."""
    sdep.reboot()

@cli.command()
def enter_dfu():
    """Force board into DFU mode."""
    sdep.enter_dfu()

@cli.command()
def factory_reset():
    """Perform a factory reset of the board."""
    sdep.syscommand(SDEP_CMD_FACTORYRESET)

@cli.command()
def nvm_reset():
    """Perform a non-volatile memory reset of the board."""
    sdep.syscommand(SDEP_CMD_NVM_RESET)

@cli.command()
def info():
    """Print information about the connected board."""
    resp = sdep.syscommand(SDEP_CMD_INFO)
    click.echo(resp.tostring())

@cli.command()
@click.argument('binfile',
                type=click.Path(exists=True))
def featherlib_upgrade(binfile):
    """Program a .bin file to the Featherlib firmware location."""
    dfu_download_bin(dfu_util_path, binfile, featherlib_addr)

@cli.command()
@click.argument('binfile',
                type=click.Path(exists=True))
def arduino_upgrade(binfile):
    """Program a .bin file to the Arduino firmware location."""
    dfu_download_bin(dfu_util_path, binfile, arduino_addr)

if __name__ == '__main__':
    cli()
