/**************************************************************************/
/*!
    @file     adafruit_sdep.cpp
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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <Arduino.h>
#include <IPAddress.h>
#include "adafruit_sdep.h"

/******************************************************************************/
/*!
    @brief Execute SDEP with single input parameter
*/
/******************************************************************************/
bool AdafruitSDEP::sdep(uint16_t  cmd_id       ,
          uint16_t  param_len    , void const* p_param,
          uint16_t* p_result_len , void* p_result)
{
  _errno = FEATHERLIB->sdep_execute(cmd_id, param_len, p_param, p_result_len, p_result);
  handle_error(cmd_id);
  return (ERROR_NONE == _errno);
}

/******************************************************************************/
/*!
    @brief Execute SDEP with multiple input parameters
*/
/******************************************************************************/
bool AdafruitSDEP::sdep_n(uint16_t  cmd_id       ,
            uint8_t   para_count   , sdep_cmd_para_t const* para_arr,
            uint16_t* p_result_len , void* p_result)
{
  _errno = FEATHERLIB->sdep_execute_n(cmd_id, para_count, para_arr, p_result_len, p_result);
  handle_error(cmd_id);
  return (ERROR_NONE == _errno);
}

/******************************************************************************/
/*!
    @brief Get Descriptive Error String (e.g TIMEOUT)
*/
/******************************************************************************/
char const* AdafruitSDEP::errstr (void)
{
  char const* p_str = NULL;
  FEATHERLIB->sdep_execute(SDEP_CMD_ERROR_STRING, sizeof(err_t), &_errno, NULL, &p_str);
  return p_str ? p_str : "Unknown Error";
}

/******************************************************************************/
/*!
    @brief Get Descriptive Command String (e.g SDEP_CMD_UDP_PACKET_INFO)
*/
/******************************************************************************/
char const* AdafruitSDEP::cmdstr (uint16_t cmd_id)
{
  char const* p_str = NULL;
  FEATHERLIB->sdep_execute(SDEP_CMD_COMMAND_STRING, sizeof(uint16_t), &cmd_id, NULL, &p_str);
  return p_str;
}

/******************************************************************************/
/*!
    @brief Handle error according to set actions
*/
/******************************************************************************/
void AdafruitSDEP::handle_error(uint16_t cmd_id)
{
  if (_err_print && (ERROR_NONE != _errno) && (_skip_err != _errno) )
  {
    char const* cmd_str = this->cmdstr(cmd_id);
    if ( cmd_str )
    {
      Serial.printf("\r\n%s failed", cmd_str);
    }else
    {
      Serial.printf("\r\nSDEP Command ID 0x%04X failed", cmd_id);
    }
    Serial.printf(", Error: %s (%d)\r\n", errstr(), errno());
  }

  if ( _err_halt && (ERROR_NONE != _errno) && (_skip_err != _errno) )
  {
    Serial.println("\r\n--- FEATHER HALTED ---\r\n");
    while(1) delay(1);
  }

  // reset skipped error
  _skip_err = ERROR_NONE;
}

/******************************************************************************/
/*!
    @brief Several Errors are 'valid' in certain situation e.g Timeout when polling
    which should not trigger fatal/panic action. This function allow to skip the
    defined error in the next SDEP command
*/
/******************************************************************************/
void AdafruitSDEP::skip_next_error(err_t skip_err)
{
  _skip_err = skip_err;
}
