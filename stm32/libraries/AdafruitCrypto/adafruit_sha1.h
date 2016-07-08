/**************************************************************************/
/*!
    @file     adafruit_sha1.h
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

#ifndef _ADAFRUIT_SHA1_H_
#define _ADAFRUIT_SHA1_H_

#include <Arduino.h>
#include <adafruit_feather.h>

class AdafruitSHA1 : public AdafruitSDEP
{
protected:
  void* _handle;

public:
  AdafruitSHA1(void);

#if 0
  // SHA1 functions
  bool begin(void);
  bool update(uint8_t const* input, uint32_t len);
  bool update(char const* input)
  {
    return this->update(input, strlen(input));
  }
  bool stop(uint8_t output[20]);
  bool stop(char output[20])
  {
    stop((uint8_t*) output);
  }
#endif

  // HMAC SHA1 functions
  bool beginHMAC(uint8_t const * key, uint32_t keylen);
  bool beginHMAC(char const * key)
  {
    return this->beginHMAC((uint8_t const *) key, strlen(key));
  }
  bool updateHMAC(uint8_t const* input, uint32_t len);
  bool updateHMAC(char const* input)
  {
//    Serial.print(input);
    return this->updateHMAC( (uint8_t const*) input, strlen(input));
  }
  bool stopHMAC(uint8_t output[20]);
  bool stopHMAC(char output[20])
  {
    stopHMAC( (uint8_t*) output);
  }

  // One-shot generate
  bool generateHMAC(uint8_t const * key, uint32_t keylen, uint8_t const* input, uint32_t len, uint8_t output[20]);
  bool generateHMAC(char const * key, uint8_t const* input, uint32_t len, uint8_t output[20])
  {
    return this->generateHMAC((uint8_t const*) key, strlen(key), input, len, output);
  }

  bool generateHMAC(uint8_t const * key, uint32_t keylen, char const* input, uint8_t output[20])
  {
    return this->generateHMAC(key, keylen, (uint8_t const*) input, strlen(input), output);
  }

  bool generateHMAC(char const * key, char const* input, uint8_t output[20])
  {
    return this->generateHMAC((uint8_t const*) key, strlen(key), (uint8_t const*) input, strlen(input), output);
  }

};

#endif /* _ADAFRUIT_SHA1_H_ */
