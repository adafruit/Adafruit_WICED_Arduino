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

  _resp_len         = 0;
  _resp_status      = 0;

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
// RESPONSE PROCESSING
//--------------------------------------------------------------------+
/**
 * Read an line from TCP stream into buffer, '\r' and '\n' is chopped,
 * null-terminator is also added at the end of buffer
 * @param buffer  Buffer, can be null to skip line
 * @param bufsize Size of buffer
 * @return Number of bytes in the line
 */
int AdafruitHTTP::readline(char* buffer, uint16_t bufsize)
{
  int count = readUntil('\n', buffer, bufsize);

  // chop ending '\r'
  if (buffer[count-1] == '\r')
  {
    count--;
    buffer[count] = 0;
  }

  return count;
}

int AdafruitHTTP::readline(void)
{
  return readline(NULL, 0);
}

int AdafruitHTTP::readUntil(char terminator, char *buffer, uint16_t bufsize)
{
  int count = 0;

  while( (bufsize == 0) || (count < bufsize-1) )
  {
    int ch = read();

    // terminator found
    if (ch == terminator) break;

    // end of data before encountering terminator
    if (ch == EOF) break;

    // allow pass NULL to swallow data
    if (buffer)
    {
      *buffer++ = (char) ch;
    }

    count++;
  }

  if (buffer) *buffer = 0; // null terminator
  return count;
}

/**
 * Parse through the whole header. Though the HTTP status code, "content-length"
 * are still extract to get data length.
 * @return  true if successful, false otherwise
 */
bool AdafruitHTTP::respParseHeader(void)
{
  char buffer[128];

  // Header end with an empty line
  while ( readline(buffer, sizeof(buffer)) )
  {
    if ( (0 == strnicmp("HTTP/", buffer, 5)) &&
         isdigit(buffer[5]) && (buffer[6] == '.') && isdigit(buffer[7]) )
    {
      _resp_status = strtoul(buffer+9, NULL, 10);
    }
    else if ( 0 == strnicmp("content-length", buffer, 14) )
    {
      _resp_len = strtoul(buffer+16, NULL, 10);
    }
  }

  return true;
}

/**
 * Get HTTP Status code
 * @return Status code
 */
int AdafruitHTTP::respStatus(void)
{
  return _resp_status;
}

int AdafruitHTTP::respContentLength(void)
{
  return _resp_len;
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
// INTERNAL FUNCTION
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
    const char* key = keyvalues[i][0];
    const char* value = keyvalues[i][1];
    total_len += strlen(key) + 1 + (url_encode ? AdafruitUrlencode::encodeLength(value) : strlen(value));
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
      uint16_t bufsize = AdafruitUrlencode::encodeLength(value)+1;
      char* encoded_value = (char*) malloc(bufsize);

      AdafruitUrlencode::encode(value, encoded_value, bufsize);
      print(encoded_value);

      free(encoded_value);
    }else
    {
      print(value);
    }
  }
}

