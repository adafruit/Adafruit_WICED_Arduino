/**************************************************************************/
/*!
    @file     adafruit_sdep.h
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

#ifndef _ADAFRUIT_SDEP_H_
#define _ADAFRUIT_SDEP_H_

#include "adafruit_constants.h"
#include "adafruit_featherlib.h"

class AdafruitSDEP
{
protected:
  err_t _errno;
  bool  _err_print;
  bool  _err_halt;

  void  handle_error(void)
  {
    if (_err_print && (ERROR_NONE != _errno))
    {
      Serial.printf("\r\nSDEP Error: %s (%d)\r\n", errstr(), errno());
    }

    if ( _err_halt && (ERROR_NONE != _errno) )
    {
      Serial.println("\r\n--- FEATHER HALTED ---\r\n");
      while(1) delay(1);
    }
  }

public:
  AdafruitSDEP() { _errno = ERROR_NONE; _err_print = _err_halt = false; }

  bool sdep(uint16_t  cmd_id       ,
            uint16_t  param_len    , void const* p_param,
            uint16_t* p_result_len , void* p_result)
  {
    _errno = FEATHERLIB->sdep_execute(cmd_id, param_len, p_param, p_result_len, p_result);
    handle_error();
    return (ERROR_NONE == _errno);
  }

  bool sdep_n(uint16_t  cmd_id       ,
              uint8_t   para_count   , sdep_cmd_para_t const* para_arr,
              uint16_t* p_result_len , void* p_result)
  {
    _errno = FEATHERLIB->sdep_execute_n(cmd_id, para_count, para_arr, p_result_len, p_result);
    handle_error();
    return (ERROR_NONE == _errno);
  }

  err_t       errno      (void         ) { return _errno; }
  char const* errstr     (void         )
  {
    char const* p_str = NULL;
    FEATHERLIB->sdep_execute(SDEP_CMD_ERROR_STRING, sizeof(err_t), &_errno, NULL, &p_str);

    return p_str ? p_str : "Unknown Error";
  }
  void        err_actions (bool print, bool halt) { _err_print = print; _err_halt = halt; }

};

// TODO move to appropriate place
ASSERT_STATIC( sizeof(err_t) == 2 );

#endif /* _ADAFRUIT_SDEP_H_ */
