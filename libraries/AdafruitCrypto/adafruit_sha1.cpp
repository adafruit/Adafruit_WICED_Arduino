/**************************************************************************/
/*!
    @file     adafruit_sha1.cpp
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

#include "adafruit_sha1.h"

/**
 * Constructor
 */
AdafruitSHA1::AdafruitSHA1(void)
{
  _handle = NULL;
}

#if 0
/**
 * Initialize SHA1
 * @return true if succeeded, false otherwise
 */
bool AdafruitSHA1::begin(void)
{

}

/**
 * Update SHA1 with input data
 * @param input Data
 * @param len   Data length
 * @return true if succeeded, false otherwise
 */
bool AdafruitSHA1::update(uint8_t const* input, uint32_t len)
{

}

/**
 * Finish SHA1 process and generate output
 * @param output  buffer to hold SHA1 result
 * @return true if succeeded, false otherwise
 */
bool AdafruitSHA1::stop(uint8_t output[20])
{

}

#endif

/**
 * Initialize HMAC SHA1
 * @param key     Signing Key
 * @param keylen  Length of key
 * @return true if succeeded, false otherwise
 */
bool AdafruitSHA1::beginHMAC(uint8_t const * key, uint32_t keylen)
{
  return sdep(SDEP_CMD_HMAC_SHA1_START, keylen, key, NULL, &_handle);
}

/**
 * Update SHA1 with input data
 * @param input Data
 * @param len   Data length
 * @return true if succeeded, false otherwise
 */
bool AdafruitSHA1::updateHMAC(uint8_t const* input, uint32_t len)
{
  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4   , .p_value = _handle },
      { .len = len , .p_value = input   },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_HMAC_SHA1_UPDATE, para_count, para_arr, NULL, NULL);
}

/**
 * Finish HMAC SHA1 process and generate output
 * @param output buffer to hold SHA1 result
 * @return true if succeeded, false otherwise
 */
bool AdafruitSHA1::stopHMAC(uint8_t output[20])
{
  bool result = sdep(SDEP_CMD_HMAC_SHA1_FINISH, 4, _handle, NULL, output);
  _handle = NULL;

  return result;
}

/**
 * Generate HMAC SHA1 in one function
 * @param key       Signing key
 * @param keylen    Length of key
 * @param input     Input data
 * @param len       Length of input data
 * @param output    Buffer (20 bytes) to hold result
 * @return true if succeeded, false otherwise
 */
bool AdafruitSHA1::generateHMAC(uint8_t const * key, uint32_t keylen, uint8_t const* input, uint32_t len, uint8_t output[20])
{
  VERIFY(beginHMAC(key, keylen));
  VERIFY(updateHMAC(input, len));
  return stopHMAC(output);
}
