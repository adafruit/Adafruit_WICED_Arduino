/**************************************************************************/
/*!
    @file     adafruit_base64.cpp
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

#include "adafruit_base64.h"

/**
 * Encode data to base64
 * @param input     Input data
 * @param inputlen  Input length
 * @param output    Output buffer
 * @param size      Output buffer size
 * @return  Number of bytes converted, 0 if failed
 */
uint16_t AdafruitBase64::encode(const uint8_t* input, uint16_t inputlen, char* output, uint16_t bufsize)
{
  return Feather.sdep(SDEP_CMD_BASE64_ENCODE, inputlen, input, &bufsize, output) ? bufsize : 0;
}

///**
// * Decode data from base64
// * @param input     Input data
// * @param inputlen  Input length
// * @param output    Output buffer
// * @param size      Output buffer size
// * @return  Number of bytes converted, 0 if failed
// */
//uint16_t AdafruitBase64::base64Decode(const char* input, uint16_t inputlen, uint8_t* output, uint16_t size)
//{
//  return Feather.sdep(SDEP_CMD_BASE64_DECODE, inputlen, input, &size, output) ? size : 0;
//}
