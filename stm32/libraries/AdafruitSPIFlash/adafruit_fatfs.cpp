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

const char VOLUME_LABEL[] = "SPI FLASH";
const char README_CONTENT[] =
"This drive can be used to add or remove files to the on-board 16 Mbit (2 MB)\r\n"
"SPI flash IC on the Adafruit WICED Feather.\r\n"
"\r\n"
"For information on how to use the WICED Feather, see the following links:\r\n"
"\r\n"
"Adafruit WICED Feather Learning Guide:\r\n"
"https://learn.adafruit.com/introducing-the-adafruit-wiced-feather-wifi\r\n"
"\r\n"
"Adafruit WICED Feather Product Page:\r\n"
"https://www.adafruit.com/product/3056\r\n";

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
 * Erase Physical Disk to blank
 */
/******************************************************************************/
void AdafruitFatfs::eraseAll(void)
{
  this->stop(); // stop any working, user have to call begin() again

  AdafruitSDEP sdep;
  sdep.sdep(SDEP_CMD_SFLASH_ERASEALL, 0, NULL, NULL, NULL);

  // Notify PC Host that filesystem has changed
  FEATHERLIB->sdep_execute(SDEP_CMD_USBMSC_FORCE_REFRESH, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/**
 * Make Filesystem a.k.a Format
 */
/******************************************************************************/
bool AdafruitFatfs::mkfs(void)
{
  void* workbuf = malloc(_MAX_SS);
  error = f_mkfs("", FM_FAT | FM_SFD, _MAX_SS, workbuf, _MAX_SS);
  free(workbuf);

  VERIFY(FR_OK == error);

  // Notify PC Host that filesystem has changed
  FEATHERLIB->sdep_execute(SDEP_CMD_USBMSC_FORCE_REFRESH, 0, NULL, NULL, NULL);

  return true;
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
  error = f_mount(_fs, "", 1);

  // If FileSystem is not available, make one
  if ( FR_NO_FILESYSTEM == error )
  {
    VERIFY ( this->mkfs() );
    setLabel(VOLUME_LABEL);

    FatFile readme;
    readme.open("readme.txt", FAT_FILE_WRITE | FAT_FILE_CREATE_ALWAYS);
    readme.write(README_CONTENT);
    readme.close();
  }else
  {
    return FR_OK == error;
  }
}

/******************************************************************************/
/**
 * Stop
 */
/******************************************************************************/
bool AdafruitFatfs::stop(void)
{
  if (_fs) free_named("FATFS", _fs);
  _fs = NULL;
}

/******************************************************************************/
/**
 * Set Volume Label
 */
/******************************************************************************/
bool AdafruitFatfs::setLabel(const char* label)
{
  error = f_setlabel(label);

  VERIFY(FR_OK == error);

  // Notify PC Host that filesystem has changed
  FEATHERLIB->sdep_execute(SDEP_CMD_USBMSC_FORCE_REFRESH, 0, NULL, NULL, NULL);

  return true;
}

/******************************************************************************/
/**
 * Get Volume Label
 */
/******************************************************************************/
bool AdafruitFatfs::getLabel(char* label)
{
  error = f_getlabel(NULL, label, NULL);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Open a directory in the filesystem
 */
/******************************************************************************/
FatDir AdafruitFatfs::openDir(const char *path)
{
  FatDir fd;
  error = f_opendir(&fd._dir, path);

  return fd;
}

/******************************************************************************/
/**
 * Open a directory in the filesystem
 */
/******************************************************************************/
bool AdafruitFatfs::openDir(const char* path, FatDir* dir)
{
  error = f_opendir(&dir->_dir, path);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Close a opened directory
 */
/******************************************************************************/
bool AdafruitFatfs::closeDir(FatDir* dir)
{
  error = f_closedir(&dir->_dir);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Check if path is existed. Root is also consider existed
 */
/******************************************************************************/
bool AdafruitFatfs::exists(const char* path)
{
  // Root is considered as true
  if ( !strcmp(path, "/") ) return true;

  error = f_stat(path, NULL);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Check if path is a folder
 */
/******************************************************************************/
bool AdafruitFatfs::isDirectory(const char* path)
{
  if ( !strcmp(path, "/")  ) return true;

  FileInfo finfo;
  error = f_stat(path, &finfo._info);
  VERIFY( FR_OK == error );
  return finfo.isDirectory();
}

/******************************************************************************/
/**
 * Get a File Information
 */
/******************************************************************************/
bool AdafruitFatfs::fileInfo(const char* path, FileInfo* finfo)
{
  error = f_stat(path, &finfo->_info);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Change current directory in filesystem
 */
/******************************************************************************/
bool AdafruitFatfs::cd(const char* path)
{
  error = f_chdir(path);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Get current working directory in absolute path
 */
/******************************************************************************/
bool AdafruitFatfs::cwd(char* buffer, uint32_t bufsize)
{
  error = f_getcwd(buffer, (UINT) bufsize);
  return FR_OK == error;
}

/******************************************************************************/
/**
 * Create specified directory, create parent directory if not existed, much like
 *  mkdir -p path
 * @param path  directory path
 * @return true if successful
 */
/******************************************************************************/
bool AdafruitFatfs::mkdir(const char* path)
{
  error = f_mkdir(path);

  VERIFY(FR_OK == error);

  // Notify PC Host that filesystem has changed
  FEATHERLIB->sdep_execute(SDEP_CMD_USBMSC_FORCE_REFRESH, 0, NULL, NULL, NULL);

  return true;
}

/******************************************************************************/
/**
 *
 *  If condition of the object to be removed is applicable to the following terms,
 *  the function will be rejected.
 *    - The file/sub-directory must not have read-only attribute (AM_RDO),
 *      or the function will be rejected with FR_DENIED.
 *    - The sub-directory must be empty and must not be current directory,
 *      or the function will be rejected with FR_DENIED.
 *    - The file/sub-directory must not be opened, or the FAT volume can be
 *    collapsed. It will be rejected safely when file lock function is enabled.
 *
 * @param path
 * @return true if successful
 */
/******************************************************************************/
bool AdafruitFatfs::remove(const char* path)
{
  error = f_unlink(path);

  VERIFY(FR_OK == error);

  // Notify PC Host that filesystem has changed
  FEATHERLIB->sdep_execute(SDEP_CMD_USBMSC_FORCE_REFRESH, 0, NULL, NULL, NULL);

  return true;
}

/******************************************************************************/
/**
 * Rename or Move a file/folder
 * @param path_old
 * @param path_new
 * @return true if successful
 */
/******************************************************************************/
bool AdafruitFatfs::rename(const char* path_old, const char* path_new)
{
  error = f_rename(path_old, path_new);
  VERIFY(FR_OK == error);

  // Notify PC Host that filesystem has changed
  FEATHERLIB->sdep_execute(SDEP_CMD_USBMSC_FORCE_REFRESH, 0, NULL, NULL, NULL);

  return true;
}

