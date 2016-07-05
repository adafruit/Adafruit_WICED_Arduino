/**************************************************************************/
/*!
    @file     adafruit_http.cpp
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

#include "adafruit_http.h"

#define HTTP_GET      "GET"
#define HTTP_POST     "POST"
#define HTTP_VERSION  "HTTP/1.1"

/**
 * Constructor
 */
AdafruitHTTP::AdafruitHTTP()
{
  _packet_buffering = true;
  _server           = NULL;
  _verbose          = false;

  this->clearHeaders();
}

/**
 *
 * @param name
 * @param value
 * @return
 */
bool AdafruitHTTP::addHeader(const char* name, const char* value)
{
  if (_header_count >= ADAFRUIT_HTTP_MAX_HEADER) return false;

  _headers[_header_count].name  = name;
  _headers[_header_count].value = value;

  _header_count++;
  return true;
}

/**
 *
 * @return
 */
bool AdafruitHTTP::clearHeaders(void)
{
  _header_count     = 0;
  for(uint8_t i=0; i<ADAFRUIT_HTTP_MAX_HEADER; i++)
  {
    _headers[i].name = _headers[i].value = NULL;
  }
}

/**
 *
 * @param content_len
 */
void AdafruitHTTP::sendHeaders(size_t content_len)
{
  for(uint8_t i=0; i<_header_count; i++)
  {
    // Avoid uisng printf since header value can be > 256 bytes
    // printf("%s: %s", _headers[i].name, _headers[i].value);
    print(_headers[i].name);
    print(": ");
    print(_headers[i].value);
    println();
  }

  // Add "Content-Length" if available
  if (content_len)
  {
    printf("Content-Length: %u", content_len);
    println();
  }

  println(); // End of header
}

/**
 *
 * @param host
 * @param url
 * @return
 */
bool AdafruitHTTP::get(char const * host, char const *url)
{
  printf(HTTP_GET " %s " HTTP_VERSION, url); println();
  printf("Host: %s", host); println();

  sendHeaders(0);
  flush();
}

/**
 *
 * @param url
 * @return
 */
bool AdafruitHTTP::get(char const *url)
{
  if(_server == NULL) return false;

  this->get(_server, url);
}

/**
 *
 * @param host
 * @param url
 * @param data
 * @param url_encode  Perfom URL Encode on Data before POST
 * @return
 */
bool AdafruitHTTP::post_internal(char const * host, char const *url, char const* data, bool url_encode)
{
  printf(HTTP_POST " %s " HTTP_VERSION, url); println();
  printf("Host: %s", host); println();

  uint16_t post_datalen = strlen(data);
  char *encoded_data = NULL;

  // Perform URL encoding if required
  if (url_encode)
  {
    uint16_t bufsize = 3*strlen(data);
    encoded_data = (char*) malloc_named("HTTP URLEncode", bufsize);
    post_datalen = urlEncode(data, encoded_data, bufsize);
  }

  sendHeaders( post_datalen );

  // send data
  if (url_encode)
  {
    println(encoded_data);
    free_named("HTTP URLEncode", encoded_data);
  }else
  {
    println(data);
  }

  flush();
}

//--------------------------------------------------------------------+
// STATIC FUNCTIONS (UTILITIES)
//--------------------------------------------------------------------+
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
uint16_t AdafruitHTTP::urlEncode(const char* input, char* output, uint16_t size)
{
  uint16_t len=0;
  char ch;
  while( (ch = *input++) && (len < size-1)  )
  {
    if ( isalnum(ch) || strchr("-_.~", ch) )
    {
      *output++ = ch;
      len++;
    }
//    else if ( ch == ' ')
//    {
//      *output++ = '+';
//      len++;
//    }
    else
    {
      *output++ = '%';
      *output++ = to_hex( ch >> 4  );
      *output++ = to_hex( ch & 0x0f);

      len += 3;
    }
  }

  // not enough memory to hold the encoded --> return 0
  if ( (len == size-1) && *input ) return 0;

  *output = 0;
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

/**
 * Encode data to base64
 * @param input     Input data
 * @param inputlen  Input length
 * @param output    Output buffer
 * @param size      Output buffer size
 * @return  Number of bytes converted, 0 if failed
 */
uint16_t AdafruitHTTP::base64Encode(const uint8_t* input, uint16_t inputlen, char* output, uint16_t size)
{
  return Feather.sdep(SDEP_CMD_BASE64_ENCODE, inputlen, input, &size, output) ? size : 0;
}

/**
 * Decode data from base64
 * @param input     Input data
 * @param inputlen  Input length
 * @param output    Output buffer
 * @param size      Output buffer size
 * @return  Number of bytes converted, 0 if failed
 */
uint16_t AdafruitHTTP::base64Decode(const char* input, uint16_t inputlen, uint8_t* output, uint16_t size)
{
  return Feather.sdep(SDEP_CMD_BASE64_DECODE, inputlen, input, &size, output) ? size : 0;
}
