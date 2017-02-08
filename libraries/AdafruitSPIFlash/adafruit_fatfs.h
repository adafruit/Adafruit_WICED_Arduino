/**************************************************************************/
/*!
    @file     adafruit_fatfs.h
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

#ifndef _ADAFRUIT_FATFS_H_
#define _ADAFRUIT_FATFS_H_

#define	FAT_FILE_OPEN_EXISTING  0x00 // Opens the file. The function fails if the file is not existing.
#define	FAT_FILE_READ           0x01 // Specifies read access to the object. Data can be read from the file.
#define	FAT_FILE_WRITE          0x02 // Specifies write access to the object. Data can be written to the file. Combine with FA_READ for read-write access.
#define	FAT_FILE_CREATE_NEW     0x04 // Creates a new file. The function fails with FR_EXIST if the file is existing.
#define	FAT_FILE_CREATE_ALWAYS  0x08 // Creates a new file. If the file is existing, it will be truncated and overwritten.
#define	FAT_FILE_OPEN_ALWAYS    0x10 // Opens the file if it is existing. If not, a new file will be created.
#define	FAT_FILE_OPEN_APPEND    0x30 // Same as FA_OPEN_ALWAYS except read/write pointer is set end of the file.

#include <Arduino.h>
#include <adafruit_feather.h>
#include "utility/ff.h"
#include "adafruit_fatdir.h"
#include "adafruit_fatfile.h"

class AdafruitFatfs
{
private:
  FATFS* _fs;

public:
  uint32_t error;

  AdafruitFatfs();

  void eraseAll(void);

  bool begin(void);
  bool stop(void);

  // Volume
  bool mkfs    (void);
  bool setLabel(const char* label);
  bool getLabel(      char* label);


  // Directory
  FatDir openDir(const char* path);
  bool   openDir(const char* path, FatDir* dir);
  bool   closeDir(FatDir* dir);

  // Test if a file/folder exists
  bool   exists   (const char* path);

  // Test if path is directory
  bool   isDirectory(const char* path);

  // Get file Information
  bool   fileInfo(const char* path, FileInfo* finfo);

  // Change current working directory
  bool cd(const char* path);

  // Get absolute path of current working directory
  bool cwd(char* buffer, uint32_t bufsize);

  // Make a directory (and its parents if not existed) a.k.a mkdir -p
  bool mkdir(const char* path);

  // Remove a non-readonly file or an empty directory
  bool remove(const char* path);
  bool rm(const char* path) { return remove(path); }

  // Rename or Move a file/folder
  bool rename(const char* path_old, const char* path_new);
  bool mv(const char* path_old, const char* path_new) { return rename(path_old, path_new); }
};

class FileInfo
{
  friend class AdafruitFatfs;
  friend class FatDir;
  friend class FatFile;

private:
  FILINFO _info;

public:
  FileInfo(void) { varclr(&_info); }
  FileInfo(const char* path) { fetch(path); }

  bool fetch(const char* path)
  {
    return (FR_OK == f_stat(path, &_info));
  }

  bool     valid       (void) { return _info.fname[0] != 0; }
  uint32_t size        (void) { return _info.fsize;         }
  uint8_t  attribute   (void) { return _info.fattrib;       }
  char*    name        (void) { return _info.fname;         }

  bool     isReadonly  (void) { return _info.fattrib & AM_RDO; }
  bool     isHidden    (void) { return _info.fattrib & AM_HID; }
  bool     isSystemFile(void) { return _info.fattrib & AM_SYS; }
  bool     isDirectory (void) { return _info.fattrib & AM_DIR; }
  bool     isArchive   (void) { return _info.fattrib & AM_ARC; }
};

#endif /* _ADAFRUIT_FATFS_H_ */
