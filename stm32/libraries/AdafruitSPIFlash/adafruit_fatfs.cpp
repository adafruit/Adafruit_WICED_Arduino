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
//#include "fatfs/diskio.h"

AdafruitFatfs::AdafruitFatfs()
{
  _fs = NULL;
}

bool AdafruitFatfs::begin()
{
  _fs = malloc_type(FATFS);

  // Mount fat filessystem
  FRESULT status = f_mount(_fs, "", 1);

  // If FileSystem is not available, make one
  if ( FR_NO_FILESYSTEM == status )
  {
    uint8_t workbuf[_MAX_SS];
    return FR_OK == f_mkfs("", FM_FAT | FM_SFD, _MAX_SS, workbuf, sizeof(workbuf));
  }else
  {
    return FR_OK == status;
  }
}

bool AdafruitFatfs::stop(void)
{
  free_named("FATFS", _fs);
  _fs = NULL;
}

FatDir AdafruitFatfs::openDir(const char *path)
{
  FatDir fd;
  f_opendir(&fd._dir, path);

  return fd;
}

//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
FatDir::FatDir(void)
{
  varclr(_dir);
}

bool FatDir::valid(void)
{
  _FDID* obj = &_dir.obj;
  return !(!obj || !obj->fs || !obj->fs->fs_type || obj->fs->id != obj->id);
}

/******************************************************************************/
/**
 * Read File Entry Information in sequence
 * @param finfo FileInfo to hold read info
 * @return true if successful
 */
/******************************************************************************/
bool FatDir::read(FatFileInfo* finfo)
{
  return (FR_OK == f_readdir(&_dir, &finfo->_info)) && (finfo->name()[0] != 0);
}

/******************************************************************************/
/**
 * Read File Entry Information in sequence
 * @return File Information object
 */
/******************************************************************************/
FatFileInfo FatDir::read(void)
{
  FatFileInfo entry;
  f_readdir(&_dir, &entry._info);
  return entry;
}

/******************************************************************************/
/**
 * Rewind Directory
 * @return true if successful
 */
/******************************************************************************/
bool FatDir::rewind(void)
{
  return FR_OK == f_readdir(&_dir, NULL);
}
