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
uint16_t AdafruitUrlencode::encode(const char* input, char* output, uint16_t bufsize)
{
  uint16_t len=0;
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
uint16_t AdafruitUrlencode::encodeLength(const char* input)
{
  uint16_t len=0;
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
/**
 * Encode URL with keys and values, each are passed in separated array
 * @param keys    Array of key which are left untouched
 * @param values  Array of value which will be urlencoded
 * @param output  Result in format
 *                key1=encoded(value1)&key2=encoded(value2)&.....
 * @param bufsize Buffer size of output
 *
 * @note If either a key or value is NULL, that pair will be skipped in the encoding result as if
 * it is not existed.
 * @return number of bytes in output string, 0 if failed (possibly not enough memory in output)
 */
uint16_t AdafruitUrlencode::encode(const char* keys[], const char* values[], uint16_t count, char* output, uint16_t bufsize)
{
  uint16_t total_bytes = 0;

  for(uint16_t i=0; i<count && total_bytes < bufsize-1; i++)
  {
    // skip NULL key or value
    if ( keys[i] && values[i] )
    {
      if (i != 0) output[total_bytes++] = '&';

      uint16_t keylen = strlen(keys[i]);
      strncpy(output+total_bytes, keys[i], bufsize-total_bytes);
      total_bytes += keylen;

      output[total_bytes++] = '=';

      uint16_t n = encode(values[i], output+total_bytes, bufsize-total_bytes);
      if (n == 0) return 0; // failed to encode

      total_bytes += n;
    }
  }

  output[total_bytes] = 0;

  return total_bytes;
}

/**
 * Encode URL with keys and values, both are passed in an 2-dimension array
 * @param keys_values  2-dimension Array of key (index 0) & value (index 1)
 * @param output  Result in format
 *                key1=encoded(value1)&key2=encoded(value2)&.....
 * @param bufsize Buffer size of output
 *
 * @note If either a key or value is NULL, that pair will be skipped in the encoding result as if
 * it is not existed.
 * @return number of bytes in output string, 0 if failed (possibly not enough memory in output)
 */
uint16_t AdafruitUrlencode::encode(const char* keys_values[][2], uint16_t count, char* output, uint16_t bufsize)
{
  uint16_t total_bytes = 0;

  for(uint16_t i=0; i<count && total_bytes < bufsize-1; i++)
  {
    char const * key   = keys_values[i][0];
    char const * value = keys_values[i][1];

    // skip NULL key or value
    if ( key && value )
    {
      if (i != 0) output[total_bytes++] = '&';
      uint16_t n = encode(&key, &value, 1, output+total_bytes, bufsize-total_bytes);
      if (n == 0) return 0; // failed to encode

      total_bytes += n;
    }
  }

  output[total_bytes] = 0;

  return total_bytes;
}
#endif

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
