/**************************************************************************/
/*!
    @file     adafruit_featherlib.h
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

#ifndef _ADAFRUIT_FEATHERLIB_H_
#define _ADAFRUIT_FEATHERLIB_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "compiler.h"
#include "assertion.h"
#include "adafruit_constants.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Define UNISTD.h replacements */
#define STDIN_FILENO          0
#define STDOUT_FILENO         1
#define STDERR_FILENO         2
#define FILENO_USB_SERIAL     3
#define FILENO_USB_ATPARSER   4
#define FILENO_SWO            5

#define FEATHERLIB_BOOTLOADER_VERSION      (*((uint32_t*) 0x08000200)) // Bootloader version saved in flash

//------------- Arduino Shared Structure -------------//
#define CFG_ARDUINO_CODE_MAGIC    0xDEC0DED
#define CFG_ARDUINO_CODE_VERSION  "0.5.5"

#define RESERVED_           XSTRING_CONCAT_(_rerserved, __LINE__)

//------------- Arduino Shared Structure -------------//
typedef struct ATTR_ALIGNED(512)
{
  // 0 - 63
  uint32_t arduino_magic;
  char     version_str[8];
  uint8_t  RESERVED_[52];

  // 64 - 127
  void (*startup) (void);
  uint32_t RESERVED_[15];

  // 128 - 191
  uint32_t RESERVED_;
  void (*cdc_serial_event_cb)(uint32_t eid, void* p_data);
  uint32_t RESERVED_[2];

  uint32_t RESERVED_[12];

  // 192 - 255 (16 functions)
  void (*wifi_connect_callback    )(void); // not used for now
  void (*wifi_disconnect_callback )(void);
  uint32_t RESERVED_[2];

  err_t (*tcpserver_connect_callback    )(void* socket, void* p_tcpserver);
  err_t (*tcp_receive_callback          )(void* socket, void* p_tcp);
  err_t (*tcp_disconnect_callback       )(void* socket, void* p_tcp);
  uint32_t RESERVED_[1];

  err_t (*udp_receive_callback          )(void* socket, void* p_udp);
  uint32_t RESERVED_[3];

  void  (*mqtt_subscribed_callback)(char* topic_data, size_t topic_len, uint8_t* message, size_t len, void* callback_func, void* arg);
  void  (*mqtt_disconnect_callback)(void* p_mqtt);
  uint32_t RESERVED_[2];

  // 256 - 512
  uint32_t RESERVED_[64];

}adafruit_arduino_t;

ASSERT_STATIC( sizeof(adafruit_arduino_t) == 512 );

//------------- Featherlib Shared Structure -------------//
typedef struct
{
  uint16_t    len;
  void const* p_value;
}sdep_cmd_para_t;

typedef struct ATTR_ALIGNED(512)
{
  // Static Information
  uint32_t firmware_magic;
  uint32_t firmware_version;
  uint32_t codebase_verion;

  char     board_name[12];         // e.g "Feather"
  char     mcu_str[12];            // e.g "ST32F205RGY"
  char     firmware_builddate[12]; // e.g "Dec 07 2015"
  uint32_t sdk_version;

  uint8_t  RESERVED_[76];

  // SDEP Command
  err_t (*sdep_execute) (uint16_t  cmd_id     ,
                         uint16_t  paylen    , void const* parameter,
                         uint16_t* result_len , void* result_buffer);

  err_t (*sdep_execute_n)(uint16_t  cmd_id       ,
                          uint8_t   para_count   , sdep_cmd_para_t const* para_arr,
                          uint16_t* p_result_len , void* p_result);

  uint32_t RESERVED_[6];

  // RTOS API
  uint32_t (*rtos_delay_ms) (uint32_t ms);
  uint32_t (*rtos_delay_us) (uint32_t us);
  void     (*rtos_yield) (void);
  uint32_t RESERVED_[13];

  // Peripheral API
  uint32_t (*system_millis) (void);
  bool     (*sflash_is_available)(void);
  uint32_t RESERVED_[14];

  // FILE Interface
  int (*file_write     ) (int file, char *ptr, int len);
  int (*file_read      ) (int file, char *ptr, int len);
  int (*file_peek      ) (int file);
  int (*file_available ) (int file);

  // DEBUG
  int32_t  (*heap_get_free_size)(void);
  uint32_t RESERVED_[3];

}adafruit_featherlib_t;

ASSERT_STATIC( sizeof(adafruit_featherlib_t) == 512 );

#define FEATHERLIB_BASE    ((uint32_t) 0x8010200)
#define FEATHERLIB         ((adafruit_featherlib_t const*) FEATHERLIB_BASE)

#define FEATHERLIB_API_EXISTS(func)  is_within(FEATHERLIB_BASE, (uint32_t) FEATHERLIB->func, 0x80DFFFF)

//--------------------------------------------------------------------+
// HELPER
//--------------------------------------------------------------------+
static inline bool is_within(uint32_t lower, uint32_t value, uint32_t upper) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline bool is_within(uint32_t lower, uint32_t value, uint32_t upper)
{
  return (lower <= value) && (value <= upper);
}

#ifdef __cplusplus
 }
#endif

#endif /* _ADAFRUIT_FEATHERLIB_H_ */
