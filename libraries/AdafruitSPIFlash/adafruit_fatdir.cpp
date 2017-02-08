/**************************************************************************/
/*!
    @file     adafruit_fatdir.cpp
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

/******************************************************************************/
/**
 * Constructor
 */
/******************************************************************************/
FatDir::FatDir(void)
{
  error = FR_OK;
  varclr(&_dir);
}

/******************************************************************************/
/**
 * Constructor
 */
/******************************************************************************/
FatDir::FatDir(const char* path)
{
  error = FR_OK;
  this->open(path);
}

/******************************************************************************/
/**
 * Open Directory
 * @return true if successful
 */
/******************************************************************************/
bool FatDir::open(const char* path)
{
  error = f_opendir(&_dir, path);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Close Directory
 * @return true if successful
 */
/******************************************************************************/
bool FatDir::close(void)
{
  error = f_closedir(&_dir);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Check if Directory is valid (bound with an folder)
 * @return true if successful
 */
/******************************************************************************/
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
bool FatDir::read(FileInfo* finfo)
{
  error = f_readdir(&_dir, &finfo->_info);
  return (FR_OK == error) && (finfo->name()[0] != 0);
}

/******************************************************************************/
/**
 * Rewind Directory
 * @return true if successful
 */
/******************************************************************************/
bool FatDir::rewind(void)
{
  error = f_rewinddir(&_dir);
  return FR_OK == error;
}

