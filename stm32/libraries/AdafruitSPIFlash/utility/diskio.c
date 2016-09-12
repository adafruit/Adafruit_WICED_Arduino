/**************************************************************************/
/*!
    @file     diskio.c
    @author   hathach (tinyusb.org)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2015, Adafruit Industries (adafruit.com)
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

//--------------------------------------------------------------------+
// INCLUDE
//--------------------------------------------------------------------+
#include <stdio.h>
#include "ff.h"
#include "diskio.h"

#include "adafruit_featherlib.h"


// Adafruit: to resolve confliction with BYTE as enum in Print.h
#define BYTE  uint8_t

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF
//--------------------------------------------------------------------+
static DSTATUS disk_state = STA_NOINIT;

//--------------------------------------------------------------------+
// INTERNAL OBJECT & FUNCTION DECLARATION
//--------------------------------------------------------------------+
#define CFG_SFLASH_CAPACITY       (2*1024*1024)
#define SFLASH_SECTOR_SIZE        4096
#define SFLASH_SECTOR_PER_BLOCK   16


DSTATUS disk_initialize ( BYTE pdrv )
{
  (void) pdrv;

  disk_state = (FEATHERLIB->sflash_enabled() ? 0 : STA_NOINIT);

  return disk_state;
}

DSTATUS disk_status (BYTE pdrv)
{
  (void) pdrv;
  return disk_state;
}

/******************************************************************************/
/**
 * Read data from DISK (SFLASH)
 * @param pdrv    Physical drive number
 * @param buff    Pointer to the byte array to store the read data.
 *                The size of buffer must be in sector size * sector count.
 * @param sector  Start sector number in logical block address (LBA).
 * @param count   Number of sectors to read. The value can be 1 to 128. Generally,
 *                a multiple sector transfer request must not be split into single
 *                sector transactions to the device, or you may not get good read performance.
 * @return        Status
 */
/******************************************************************************/
DRESULT disk_read (BYTE pdrv, BYTE*buff, DWORD sector, UINT count)
{
  (void) pdrv;
  return FEATHERLIB->sflash_read(SFLASH_SECTOR_SIZE*sector, buff, count*SFLASH_SECTOR_SIZE);
}

/******************************************************************************/
/**
 * Write data to DISK (SFLASH)
 * @param pdrv    Physical drive number
 * @param buff    Pointer to data.
 * @param sector  Start sector number in logical block address (LBA).
 * @param count   Number of sectors to write. The value can be 1 to 128. Generally,
 *                a multiple sector transfer request must not be split into single
 *                sector transactions to the device, or you may not get good read performance.
 * @return        Status
 */
/******************************************************************************/
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
  (void) pdrv;
  for(UINT i=0; i<count; i++)
  {
    FEATHERLIB->sflash_erase_sector( (sector+i)*SFLASH_SECTOR_SIZE );
  }
  return FEATHERLIB->sflash_write(SFLASH_SECTOR_SIZE*sector, buff, count*SFLASH_SECTOR_SIZE);
}

/******************************************************************************/
/**
 * Disk I/O Control
 * @param pdrv  Physical drive number
 * @param cmd   Control command code
 * @param buff  Parameter and data buffer
 * @return      Status
 */
/******************************************************************************/
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
  (void) pdrv;

  switch(cmd)
  {
    case GET_SECTOR_COUNT:
      *((DWORD*) buff) = CFG_SFLASH_CAPACITY / SFLASH_SECTOR_SIZE;
    break;

    case GET_SECTOR_SIZE:
      *((WORD*) buff) = SFLASH_SECTOR_SIZE;
    break;

    case GET_BLOCK_SIZE:
      *((DWORD*) buff) = SFLASH_SECTOR_SIZE*SFLASH_SECTOR_PER_BLOCK;
    break;

    // nothing to since each write operation to the media is completed within the disk_write function.
    case CTRL_SYNC: break;
    case CTRL_TRIM: break;

    default: return RES_ERROR;
  }

  return RES_OK;
}

static inline uint8_t month2number(char* p_ch)
{
  char const * const month_str[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

  for(uint8_t i=0; i<12; i++)
  {
    if ( strncmp(p_ch, month_str[i], 3) == 0 ) return i+1;
  }

  return 1;
}

static inline uint8_t c2i(char ch)
{
  return ch - '0';
}

/******************************************************
 * UTC timestamp to FAT timestamp conversion
 ******************************************************/
#define BASE_UTC_YEAR (1970)
#define IS_LEAP_YEAR( year ) ( ( ( year ) % 400 == 0 ) || \
                             ( ( ( year ) % 100 != 0 ) && ( ( year ) % 4 == 0 ) ) )


#define SECONDS_IN_365_DAY_YEAR  (31536000)
#define SECONDS_IN_A_DAY         (86400)
#define SECONDS_IN_A_HOUR        (3600)
#define SECONDS_IN_A_MINUTE      (60)
static const uint32_t secondsPerMonth[ 12 ] =
{
    31*SECONDS_IN_A_DAY,
    28*SECONDS_IN_A_DAY,
    31*SECONDS_IN_A_DAY,
    30*SECONDS_IN_A_DAY,
    31*SECONDS_IN_A_DAY,
    30*SECONDS_IN_A_DAY,
    31*SECONDS_IN_A_DAY,
    31*SECONDS_IN_A_DAY,
    30*SECONDS_IN_A_DAY,
    31*SECONDS_IN_A_DAY,
    30*SECONDS_IN_A_DAY,
    31*SECONDS_IN_A_DAY,
};

DWORD get_fattime (void)
{
  typedef union {
    struct {
      DWORD second : 5;
      DWORD minute : 6;
      DWORD hour   : 5;
      DWORD day    : 5;
      DWORD month  : 4;
      DWORD year   : 7;
    };

    DWORD value;
  } fat_timestamp_t;

  //------------- UTC timestamp to FAT timestamp conversion -------------//
  uint16_t year;
  uint16_t number_of_leap_years;
  uint8_t  month;
  uint8_t  day;
  uint8_t  hour;
  uint8_t  minute;
  uint64_t second;
  bool     is_a_leap_year;

  // Get UTC time in seconds
  FEATHERLIB->sdep_execute(SDEP_CMD_GET_UTC_TIME, 0, NULL, NULL, &second);

  /* Calculate year */
  year = (uint16_t)( BASE_UTC_YEAR + second / SECONDS_IN_365_DAY_YEAR );
  number_of_leap_years = ( uint16_t )( ( ( year - ( BASE_UTC_YEAR - ( BASE_UTC_YEAR % 4 ) + 1 ) ) / 4 ) -
      ( ( year - ( BASE_UTC_YEAR - ( BASE_UTC_YEAR % 100 ) + 1 ) ) / 100 ) +
      ( ( year - ( BASE_UTC_YEAR - ( BASE_UTC_YEAR % 400 ) + 1 ) ) / 400 ) );
  second -= (uint64_t)( (uint64_t)( year - BASE_UTC_YEAR ) * SECONDS_IN_365_DAY_YEAR );

  if ( second >= ( uint64_t )( number_of_leap_years * SECONDS_IN_A_DAY ) )
  {
    second -= (uint64_t) ( ( number_of_leap_years * SECONDS_IN_A_DAY ) );
  }
  else
  {
    do
    {
      second += SECONDS_IN_365_DAY_YEAR;
      year--;
      if ( IS_LEAP_YEAR( year ) )
      {
        second += SECONDS_IN_A_DAY;
      }
    } while ( second < ( uint64_t )( number_of_leap_years * SECONDS_IN_A_DAY ) );

    second -= ( uint64_t )( number_of_leap_years * SECONDS_IN_A_DAY );
  }

  /* Remember if the current year is a leap year */
  is_a_leap_year = ( IS_LEAP_YEAR( year ) ) ? true : false;

  /* Calculate month */
  month = 1;

  for (uint32_t a = 0; a < 12; ++a )
  {
    uint32_t seconds_per_month = secondsPerMonth[a];
    /* Compensate for leap year */
    if ( ( a == 1 ) && is_a_leap_year )
    {
      seconds_per_month += SECONDS_IN_A_DAY;
    }
    if ( second >= seconds_per_month )
    {
      second -= seconds_per_month;
      month++;
    }
    else
    {
      break;
    }
  }

  /* Calculate day */
  day    = (uint8_t) ( second / SECONDS_IN_A_DAY );
  second -= (uint64_t) ( day * SECONDS_IN_A_DAY );
  ++day;

  /* Calculate hour */
  hour   = (uint8_t) ( second / SECONDS_IN_A_HOUR );
  second -= (uint64_t)  ( hour * SECONDS_IN_A_HOUR );

  /* Calculate minute */
  minute = (uint8_t) ( second / SECONDS_IN_A_MINUTE );
  second -= (uint64_t) ( minute * SECONDS_IN_A_MINUTE );

  fat_timestamp_t timestamp =
  {
      .second = second,
      .minute = minute,
      .hour   = hour,
      .day = day,
      .month = month,
      .year = year - 1980 // FAT time origin is 1980
  };

  return timestamp.value;
}

