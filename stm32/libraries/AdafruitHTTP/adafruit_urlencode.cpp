/**************************************************************************/
/*!
    @file     adafruit_urlencode.cpp
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

#include "adafruit_urlencode.h"

/* Converts a hex character to its integer value */
static char from_hex(char ch)
{
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
static char to_hex(char code)
{
  static char const hex[] = "0123456789ABCDEF";
  return hex[code & 15];
}

/**
 *
 * @param input   Input string
 * @param output  Output string
 * @param size    Maximum size of output string
 * @return  number of bytes in output string, 0 if failed (possibly not enough memory in output)
 */
uint32_t AdafruitUrlencode::encode(const char* input, char* output, uint32_t bufsize)
{
  uint32_t len=0;
  char ch;
  while( (ch = *input++) && (len < bufsize-1)  )
  {
    if ( isalnum(ch) || strchr("-_.~", ch) )
    {
      *output++ = ch;
      len++;
    }
    else
    {
      *output++ = '%';
      *output++ = to_hex( ch >> 4  );
      *output++ = to_hex( ch & 0x0f);

      len += 3;
    }
  }

  *output = 0;

  // not enough memory to hold the encoded --> return 0
  if ( ch && (len == bufsize-1) ) return 0;

  return len;
}

/**
 * Get length of url encoded without perform the encoding
 * @param input Input string
 * @return
 */
uint32_t AdafruitUrlencode::encodeLength(const char* input)
{
  uint32_t len=0;
  char ch;
  while( (ch = *input++) )
  {
    if ( isalnum(ch) || strchr("-_.~", ch) )
    {
      len++;
    } else
    {
      len += 3;
    }
  }
  return len;
}

#if 0
/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(char *str) {
  char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } else if (*pstr == '+') {
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}
#endif
