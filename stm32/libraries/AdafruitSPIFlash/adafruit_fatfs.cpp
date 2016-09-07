/**************************************************************************/
/*!
    @file     adafruit_fatfs.cpp
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#include "adafruit_fatfs.h"

#define DEFAULT_LABEL   "SPI FLASH"

/******************************************************************************/
/**
 * Constructor
 */
/******************************************************************************/
AdafruitFatfs::AdafruitFatfs()
{
  _fs = NULL;
}

/******************************************************************************/
/**
 * Begin
 */
/******************************************************************************/
bool AdafruitFatfs::begin(void)
{
  _fs = malloc_type(FATFS);

  // Mount fat filessystem
  FRESULT status = f_mount(_fs, "", 1);

  // If FileSystem is not available, make one
  if ( FR_NO_FILESYSTEM == status )
  {
    uint8_t workbuf[_MAX_SS];
    VERIFY( FR_OK == f_mkfs("", FM_FAT | FM_SFD, _MAX_SS, workbuf, sizeof(workbuf)) );
    setLabel(DEFAULT_LABEL);
  }else
  {
    return FR_OK == status;
  }
}

/******************************************************************************/
/**
 * Stop
 */
/******************************************************************************/
bool AdafruitFatfs::stop(void)
{
  free_named("FATFS", _fs);
  _fs = NULL;
}

/******************************************************************************/
/**
 * Set Volume Label
 */
/******************************************************************************/
bool AdafruitFatfs::setLabel(const char* label)
{
  return FR_OK == f_setlabel(label);
}

/******************************************************************************/
/**
 * Get Volume Label
 */
/******************************************************************************/
bool AdafruitFatfs::getLabel(char* label)
{
  return FR_OK == f_getlabel(NULL, label, NULL);
}

/******************************************************************************/
/**
 * Open a directory in the filesystem
 */
/******************************************************************************/
FatDir AdafruitFatfs::openDir(const char *path)
{
  FatDir fd;
  f_opendir(&fd._dir, path);

  return fd;
}

/******************************************************************************/
/**
 * Open a directory in the filesystem
 */
/******************************************************************************/
bool AdafruitFatfs::openDir(const char* path, FatDir* dir)
{
  return FR_OK == f_opendir(&dir->_dir, path);
}

/******************************************************************************/
/**
 * Close a opened directory
 */
/******************************************************************************/
bool AdafruitFatfs::closeDir(FatDir* dir)
{
  return FR_OK == f_closedir(&dir->_dir);
}

/******************************************************************************/
/**
 * Change current directory in filesystem
 */
/******************************************************************************/
bool AdafruitFatfs::cd(const char* path)
{
  return FR_OK == f_chdir(path);
}

/******************************************************************************/
/**
 * Get current working directory in absolute path
 */
/******************************************************************************/
bool AdafruitFatfs::pwd(char* buffer, uint32_t bufsize)
{
  return FR_OK == f_getcwd(buffer, (UINT) bufsize);
}

/******************************************************************************/
/**
 * Get a File Information
 */
/******************************************************************************/
bool AdafruitFatfs::fileInfo(const char* path, FileInfo* finfo)
{
  return FR_OK == f_stat(path, &finfo->_info);
}

/******************************************************************************/
/**
 * Get a File Information
 */
/******************************************************************************/
FileInfo AdafruitFatfs::fileInfo(const char* path)
{
  FileInfo finfo;
  (void) f_stat(path, &finfo._info);
  return finfo;
}
