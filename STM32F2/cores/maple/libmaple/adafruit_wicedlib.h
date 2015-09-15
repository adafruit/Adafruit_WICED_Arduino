/**************************************************************************/
/*!
    @file     adafruit_wicedlib.h
    @author   hathach

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

#ifndef _ADAFRUIT_WICEDLIB_H_
#define _ADAFRUIT_WICEDLIB_H_

#include <stdint.h>
#include "compiler.h"

#ifdef __cplusplus
 extern "C" {
#endif

//------------- Arduino Shared Structure -------------//
#define CFG_ARDUINO_CODE_MAGIC    0x01ADD1E5 // Laddies

typedef struct ATTR_ALIGNED(512)
{
  uint32_t arduino_magic;
  uint8_t  reserved[60];

  void (*startup) (void);
}adafruit_arduino_t;


//------------- Wicedlib Shared Structure -------------//
typedef uint32_t wiced_result_t;

typedef struct ATTR_ALIGNED(512)
{
  // Information
  uint32_t firmware_magic;
  uint32_t firmware_version;
  uint8_t  reserved[56];

  // RTOS API
  wiced_result_t (*rtos_delay_ms) (uint32_t ms);
  wiced_result_t (*rtos_delay_us) (uint32_t us);

  // Peripheral API
  uint32_t (*system_millis) (void);

  wiced_result_t (*gpio_toggle) (uint32_t gpio);

  // FILE Interface
  int (*file_write)(int file, char *ptr, int len);
  int (*file_read)(int file, char *ptr, int len);
  int (*file_peek)(void);

  // SDEP Command
  uint16_t (*wiced_sdep) (uint16_t sdep_cmd_id, uint8_t paylen, uint8_t* payload_buffer, uint8_t* result_buffer);
}adafruit_wicedlib_t;

#define ADAFRUIT_WICEDLIB_BASE    ((uint32_t) 0x8010200)
#define ADAFRUIT_WICEDLIB         ((adafruit_wicedlib_t*) ADAFRUIT_WICEDLIB_BASE)


#ifdef __cplusplus
 }
#endif

#endif /* _ADAFRUIT_WICEDLIB_H_ */
