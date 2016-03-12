/**************************************************************************/
/*!
    @file     adafruit_utf8string.h
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

#ifndef _ADAFRUIT_UTF8STRING_H_
#define _ADAFRUIT_UTF8STRING_H_

#include <Arduino.h>
#include <Printable.h>

class UTF8String : public Printable
{
public:
  int len;
  const char* data;

  UTF8String(void) { len = 0; data = NULL; }

  UTF8String(const char* cstring            ) { len = strlen(cstring); data = cstring;  }
  UTF8String(const char* bytes, int size    ) { len = size; data = bytes;               }
  UTF8String(const uint8_t* bytes, int size ) { len = size; data = (const char*) bytes; }

  UTF8String& operator = (const UTF8String& rhs)
  {
    if (this == &rhs) return *this;

    this->len = rhs.len;
    this->data = rhs.data;

    return *this;
  }

  bool operator == (const char *cstr) const { return (strlen(cstr) == len) && !memcmp(data, cstr, len); }
  bool operator != (const char *cstr) const { return !((*this) == cstr); }

  virtual size_t printTo(Print& p) const
  {
    p.write(this->data, len);
  }

  explicit operator const char*() const { return data; }
  explicit operator const uint8_t*() const { return ( const uint8_t*) data; }
  explicit operator const unsigned char*() const { return (const unsigned char*) data; }
};


#endif /* _ADAFRUIT_UTF8STRING_H_ */
