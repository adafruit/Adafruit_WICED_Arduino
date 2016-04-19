# Boilerplate setup.py generated from cxfreeze-quickstart script.
# Use the 'build_exe' option to generate an executable for the current platform.
# For example run 'python setup.py build_exe'
# See cx_Freeze's documentation for more information:
#  http://cx-freeze.readthedocs.org/en/latest/distutils.html
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need
# fine tuning.
buildOptions = dict(packages = [], excludes = [])

base = 'Console'

executables = [
    Executable('feather_dfu.py', base=base)
]

setup(name='feather_dfu',
      version = '1.0',
      description = 'Adafruit Feather Utility',
      options = dict(build_exe = buildOptions),
      executables = executables)
