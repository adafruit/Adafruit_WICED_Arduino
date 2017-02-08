/**************************************************************************/
/*!
    @file     adafruit_fatfile.cpp
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
FatFile::FatFile(void)
{
  error = FR_OK;
  varclr(&_file);
}

/******************************************************************************/
/**
 * Constructor
 */
/******************************************************************************/
FatFile::FatFile(const char* path, uint8_t mode)
{
  error = FR_OK;
  this->open(path, mode);
}

/******************************************************************************/
/**
 * Open a FAT file
 */
/******************************************************************************/
bool FatFile::open(const char* path, uint8_t mode)
{
  error = f_open(&_file, path, mode);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Close file
 */
/******************************************************************************/
bool FatFile::close(void)
{
  error = f_close(&_file);

  // Notify PC Host that filesystem has changed
  FEATHERLIB->sdep_execute(SDEP_CMD_USBMSC_FORCE_REFRESH, 0, NULL, NULL, NULL);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Read a byte from file
 */
/******************************************************************************/
int FatFile::read( void )
{
  uint8_t data;
  return (this->read(&data, 1) == 1) ? data : EOF;
}

/******************************************************************************/
/**
 * Read data from file
 */
/******************************************************************************/
int FatFile::read( uint8_t * buf, size_t size )
{
  UINT count = 0;
  error = f_read(&_file, buf, size, &count);
  return count;
}

/******************************************************************************/
/**
 * Write a byte to file
 */
/******************************************************************************/
size_t FatFile::write( uint8_t b )
{
  return this->write(&b, 1);
}

/******************************************************************************/
/**
 * Write data to file
 */
/******************************************************************************/
size_t FatFile::write( const uint8_t *content, size_t len )
{
  UINT count = 0;
  error = f_write(&_file, content, len, &count);
  return count;
}

/******************************************************************************/
/**
 * Get remaining bytes
 */
/******************************************************************************/
int FatFile::available( void )
{
  return f_size(&_file) - f_tell(&_file);
}

/******************************************************************************/
/**
 * Get the current read/write pointer
 * @param offset  offset to current position
 * @return true if successful
 */
/******************************************************************************/
uint32_t FatFile::tell(void)
{
  return f_tell(&_file);
}

/******************************************************************************/
/**
 * Move read/write pointer to absolute offset
 * @param offset  offset to top of the file
 * @return true if successful
 */
/******************************************************************************/
bool FatFile::seek(uint32_t offset)
{
  error = f_lseek(&_file, offset);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Move read/write pointer forward
 * @param offset  offset to current position
 * @return true if successful
 */
/******************************************************************************/
bool FatFile::seekForward(uint32_t offset)
{
  error = f_lseek(&_file, tell() + offset);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Move read/write pointer backward
 * @param offset  offset to current position
 * @return true if successful
 */
/******************************************************************************/
bool FatFile::seekBackward(uint32_t offset)
{
  error = f_lseek(&_file, tell() - offset);
  return FR_OK == error;
}


/******************************************************************************/
/**
 * Get a byte without removing it
 */
/******************************************************************************/
int FatFile::peek( void )
{
  int ch = read();
  seekBackward(1);
  return ch;
}

/******************************************************************************/
/**
 * Flush any caching
 */
/******************************************************************************/
void FatFile::flush( void )
{

}


