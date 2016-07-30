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

  return true;
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

//--------------------------------------------------------------------+
// TCP API INTERFACE
//--------------------------------------------------------------------+
int AdafruitHTTP::connect( const char * host, uint16_t port )
{
  _server = host;
  return AdafruitTCP::connect(host, port);
}

int AdafruitHTTP::connectSSL( const char* host, uint16_t port )
{
  _server = host;
  return AdafruitTCP::connectSSL(host, port);
}

void AdafruitHTTP::stop( void )
{
  _packet_buffering = true;
  _server           = NULL;
  _verbose          = false;
  this->clearHeaders();

  return AdafruitTCP::stop();
}

size_t AdafruitHTTP::write( uint8_t b)
{
  if (_verbose) Serial.write(b);
  return AdafruitTCP::write(b);
}

size_t AdafruitHTTP::write( const uint8_t *content, size_t len )
{
  if (_verbose) Serial.write(content, len);
  return AdafruitTCP::write(content, len);
}

//--------------------------------------------------------------------+
// GET
//--------------------------------------------------------------------+
bool AdafruitHTTP::get(char const *host, char const *url, const char* keyvalues[][2], uint16_t count)
{
  printf(HTTP_METHOD_GET " %s", url);
  if ( count )
  {
    print('?');
    send_keyvalues_data(keyvalues, count, true);
  }
  println(" " HTTP_VERSION);
  printf("Host: %s", host); println();

  sendHeaders(0);
  flush();

  return true;
}

bool AdafruitHTTP::get(char const *url, const char* keyvalues[][2], uint16_t count)
{
  return get(_server, url, keyvalues, count);
}

bool AdafruitHTTP::get(char const * host, char const *url)
{
  return this->get(host, url, NULL, 0);
}

bool AdafruitHTTP::get(char const *url)
{
  return this->get(_server, url, NULL, 0);
}

//--------------------------------------------------------------------+
// POST WITH URLENCODING DATA
//--------------------------------------------------------------------+
bool AdafruitHTTP::post(char const *host, char const *url, const char* keyvalues[][2], uint16_t count)
{
  return post_internal(host, url, keyvalues, count, true);
}

bool AdafruitHTTP::post(char const * host, char const *url, char const* key, char const* value)
{
  const char* keyvalues[][2] = { key, value };
  return post_internal(host, url, keyvalues, 1, true);
}

bool AdafruitHTTP::post(char const *url, const char* keyvalues[][2], uint16_t count)
{
  return post(_server, url, keyvalues, count);
}

bool AdafruitHTTP::post(char const *url, char const* key, char const* value)
{
  return post(_server, url, key, value);
}

//--------------------------------------------------------------------+
// POST WITHOUT URLENCODING DATA
//--------------------------------------------------------------------+
bool AdafruitHTTP::postWithoutURLencoded(char const * host, char const *url, const char* keyvalues[][2], uint16_t count)
{
  return post_internal(host, url, keyvalues, count, false);
}

bool AdafruitHTTP::postWithoutURLencoded(char const * host, char const *url, char const* key, char const* value)
{
  const char* keyvalues[][2] = { key, value };
  return post_internal(host, url, keyvalues, 1, false);
}

bool AdafruitHTTP::postWithoutURLencoded(char const *url, const char* keyvalues[][2], uint16_t count)
{
  return postWithoutURLencoded(_server, url, keyvalues, count);
}

bool AdafruitHTTP::postWithoutURLencoded(char const *url, char const* key, char const* value)
{
  return postWithoutURLencoded(_server, url, key, value);
}

//--------------------------------------------------------------------+
// POST WITH RAW DATA
//--------------------------------------------------------------------+
bool AdafruitHTTP::postRaw(char const * host, char const *url, char const* raw_data)
{
  return postRaw(host, url, (uint8_t const*) raw_data, strlen(raw_data));
}

bool AdafruitHTTP::postRaw(char const *url, uint8_t const* raw_data, uint16_t len )
{
  return postRaw(_server, url, raw_data, len);
}

bool AdafruitHTTP::postRaw(char const *url, char const* raw_data)
{
  return postRaw(url, (uint8_t const*) raw_data, strlen(raw_data));
}

/**
 * POST with raw data useful for binary POST
 * @param host
 * @param url
 * @param raw_data
 * @param len
 * @return
 */
bool AdafruitHTTP::postRaw(char const * host, char const *url, uint8_t const* raw_data, uint16_t len)
{
  printf(HTTP_METHOD_POST " %s " HTTP_VERSION, url); println();
  printf("Host: %s", host); println();

  sendHeaders( len  );

  // send data
  write(raw_data, len);

  flush();

  return true;
}

//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
/**
 *
 * @param host
 * @param url
 * @param keyvalues
 * @param data_count
 * @param url_encode  Perfom URL Encode on Data before POST
 * @note
 *      POST Data is in form key1=value1&key2=value2
 *      where value[] could be url_encoded or not
 * @return
 */
bool AdafruitHTTP::post_internal(char const * host, char const *url, const char* keyvalues[][2], uint16_t count, bool url_encode)
{
  printf(HTTP_METHOD_POST " %s " HTTP_VERSION, url); println();
  printf("Host: %s", host); println();

  // Determine the total data length
  uint16_t total_len = 0;
  for(uint16_t i = 0; i<count; i++)
  {
    total_len += (strlen(keyvalues[i][0]) + 1) + (url_encode ? urlEncodeLength(keyvalues[i][1]) : strlen(keyvalues[i][1]));
  }
  total_len += count-1; // number of "&" between each key=value

  // Send all headers
  sendHeaders( total_len );

  // send data
  send_keyvalues_data(keyvalues, count, url_encode);

  println();
  flush();

  return true;
}

void AdafruitHTTP::send_keyvalues_data(const char* keyvalues[][2], uint16_t count, bool url_encode)
{
  for(uint16_t i = 0; i<count; i++)
  {
    if (i != 0) print("&");

    char const* key   = keyvalues[i][0];
    char const* value = keyvalues[i][1];

    print(key); print("=");

    if (url_encode)
    {
      uint16_t bufsize = urlEncodeLength(value)+1;
      char* encoded_value = (char*) malloc(bufsize);

      urlEncode(value, encoded_value, bufsize);
      print(encoded_value);

      free(encoded_value);
    }else
    {
      print(value);
    }
  }
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
uint16_t AdafruitHTTP::urlEncode(const char* input, char* output, uint16_t bufsize)
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
uint16_t AdafruitHTTP::urlEncode(const char* keys[], const char* values[], uint16_t count, char* output, uint16_t bufsize)
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

      uint16_t n = urlEncode(values[i], output+total_bytes, bufsize-total_bytes);
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
uint16_t AdafruitHTTP::urlEncode(const char* keys_values[][2], uint16_t count, char* output, uint16_t bufsize)
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
      uint16_t n = urlEncode(&key, &value, 1, output+total_bytes, bufsize-total_bytes);
      if (n == 0) return 0; // failed to encode

      total_bytes += n;
    }
  }

  output[total_bytes] = 0;

  return total_bytes;
}
#endif
/**
 * Get length of url encoded without perform the encoding
 * @param input Input string
 * @return
 */
uint16_t AdafruitHTTP::urlEncodeLength(const char* input)
{
  uint16_t len=0;
  char ch;
  while( (ch = *input++) )
  {
    if ( isalnum(ch) || strchr("-_.~", ch) /*|| ch == ' '*/ )
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
