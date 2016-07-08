/**************************************************************************/
/*!
    @file     adafruit_http.h
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

#ifndef _ADAFRUIT_HTTP_H_
#define _ADAFRUIT_HTTP_H_

#include <Arduino.h>
#include <Client.h>
#include <IPAddress.h>
#include <adafruit_feather.h>

#define ADAFRUIT_HTTP_MAX_HEADER  10

#define HTTP_METHOD_GET         "GET"
#define HTTP_METHOD_POST        "POST"
#define HTTP_VERSION            "HTTP/1.1"

#define HTTP_HEADER_URLENCODING "application/x-www-form-urlencoded"


class AdafruitHTTP : public AdafruitTCP
{
protected:
  const char* _server;

  uint8_t _header_count;
  struct {
    const char* name;
    const char* value;
  }_headers[ADAFRUIT_HTTP_MAX_HEADER];

  void sendHeaders(size_t content_len);

  bool post_internal(char const * host, char const *url, const char* keyvalues[][2], uint16_t count, bool url_encode);

private:
  bool _verbose;

public:
  AdafruitHTTP();

  void verbose(bool enable) { _verbose = enable; }

  bool addHeader(const char* name, const char* value);
  bool clearHeaders(void);

  // GET
  bool get(char const * host, char const *url);
  bool get(char const *url);

  // POST with urlencoding data
  bool post(char const *host, char const *url, const char* keyvalues[][2], uint16_t count)
  {
    return post_internal(host, url, keyvalues, count, true);
  }

  bool post(char const * host, char const *url, char const* key, char const* value)
  {
    const char* keyvalues[][2] = { key, value };
    return post_internal(host, url, keyvalues, 1, true);
  }

  bool post(char const *url, const char* keyvalues[][2], uint16_t count)
  {
    return post(_server, url, keyvalues, count);
  }

  bool post(char const *url, char const* key, char const* value)
  {
    return post(_server, url, key, value);
  }


  // POST without urlencoded
  bool postWithoutURLencoded(char const * host, char const *url, const char* keyvalues[][2], uint16_t count)
  {
    return post_internal(host, url, keyvalues, count, false);
  }

  bool postWithoutURLencoded(char const * host, char const *url, char const* key, char const* value)
  {
    const char* keyvalues[][2] = { key, value };
    return post_internal(host, url, keyvalues, 1, false);
  }

  bool postWithoutURLencoded(char const *url, const char* keyvalues[][2], uint16_t count)
  {
    return postWithoutURLencoded(_server, url, keyvalues, count);
  }

  bool postWithoutURLencoded(char const *url, char const* key, char const* value)
  {
    return postWithoutURLencoded(_server, url, key, value);
  }

  // POST with Raw data, useful with binary
  bool postRaw(char const * host, char const *url, uint8_t const* raw_data, uint16_t len );

  bool postRaw(char const * host, char const *url, char const* raw_data)
  {
    return postRaw(host, url, (uint8_t const*) raw_data, strlen(raw_data));
  }

  bool postRaw(char const *url, uint8_t const* raw_data, uint16_t len )
  {
    return postRaw(_server, url, raw_data, len);
  }

  bool postRaw(char const *url, char const* raw_data)
  {
    return postRaw(url, (uint8_t const*) raw_data, strlen(raw_data));
  }

  // TCP API
  virtual int connect    ( const char * host, uint16_t port )
  {
    _server = host;
    return AdafruitTCP::connect(host, port);
  }
  virtual int connectSSL ( const char* host, uint16_t port )
  {
    _server = host;
    return AdafruitTCP::connectSSL(host, port);
  }

  virtual void stop       ( void )
  {
    _packet_buffering = true;
    _server           = NULL;
    _verbose          = false;
    this->clearHeaders();

    return AdafruitTCP::stop();
  }

  virtual size_t    write      ( uint8_t b)
  {
    if (_verbose) Serial.write(b);
    return AdafruitTCP::write(b);
  }
  virtual size_t    write      ( const uint8_t *content, size_t len )
  {
    if (_verbose) Serial.write(content, len);
    return AdafruitTCP::write(content, len);
  }

  static uint16_t urlEncodeLength(const char* input);
  static uint16_t urlEncode(const char* input, char* output, uint16_t bufsize);
#if 0
  static uint16_t urlEncode(const char* keys[], const char* values[], uint16_t count, char* output, uint16_t bufsize);
  static uint16_t urlEncode(const char* keys_values[][2], uint16_t count, char* output, uint16_t bufsize);
#endif
//  static uint16_t urlDecode(const char* input, char* output, uint16_t bufsize);

  static uint16_t base64Encode(const uint8_t* input, uint16_t inputlen, char* output, uint16_t bufsize);
  static uint16_t base64Encode(const char* input   , uint16_t inputlen, char* output, uint16_t bufsize)
  {
    return base64Encode( (const uint8_t*) input, inputlen, output, bufsize);
  }

//  static uint16_t base64Decode(const char   * input, uint16_t inputlen, uint8_t* output, uint16_t size);
};


typedef enum
{
  HTTP_STATUS_CONTINUE                        = 100,
  HTTP_STATUS_SWITCHING_PROTOCOLS             = 101,
  HTTP_STATUS_RESPONSE_OK                     = 200,
  HTTP_STATUS_CREATED                         = 201,
  HTTP_STATUS_ACCEPTED                        = 202,
  HTTP_STATUS_NONAUTHORITATIVE                = 203,
  HTTP_STATUS_NO_CONTENT                      = 204,
  HTTP_STATUS_RESET_CONTENT                   = 205,
  HTTP_STATUS_PARTIAL_CONTENT                 = 206,
  HTTP_STATUS_MULTIPLE_CHOICES                = 300,
  HTTP_STATUS_MOVED_PERMANENTLY               = 301,
  HTTP_STATUS_FOUND                           = 302,
  HTTP_STATUS_SEE_OTHER                       = 303,
  HTTP_STATUS_NOT_MODIFIED                    = 304,
  HTTP_STATUS_USEPROXY                        = 305,
  HTTP_STATUS_TEMPORARY_REDIRECT              = 307,
  HTTP_STATUS_BAD_REQUEST                     = 400,
  HTTP_STATUS_UNAUTHORIZED                    = 401,
  HTTP_STATUS_PAYMENT_REQUIRED                = 402,
  HTTP_STATUS_FORBIDDEN                       = 403,
  HTTP_STATUS_NOT_FOUND                       = 404,
  HTTP_STATUS_METHOD_NOT_ALLOWED              = 405,
  HTTP_STATUS_NOT_ACCEPTABLE                  = 406,
  HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED   = 407,
  HTTP_STATUS_REQUEST_TIMEOUT                 = 408,
  HTTP_STATUS_CONFLICT                        = 409,
  HTTP_STATUS_GONE                            = 410,
  HTTP_STATUS_LENGTH_REQUIRED                 = 411,
  HTTP_STATUS_PRECONDITION_FAILED             = 412,
  HTTP_STATUS_REQUESTENTITYTOOLARGE           = 413,
  HTTP_STATUS_REQUESTURITOOLONG               = 414,
  HTTP_STATUS_UNSUPPORTEDMEDIATYPE            = 415,
  HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
  HTTP_STATUS_EXPECTATION_FAILED              = 417,
  HTTP_STATUS_INTERNAL_SERVER_ERROR           = 500,
  HTTP_STATUS_NOT_IMPLEMENTED                 = 501,
  HTTP_STATUS_BAD_GATEWAY                     = 502,
  HTTP_STATUS_SERVICE_UNAVAILABLE             = 503,
  HTTP_STATUS_GATEWAY_TIMEOUT                 = 504,
  HTTP_STATUS_VERSION_NOT_SUPPORTED           = 505,
} http_status_code_t;

#endif /* _ADAFRUIT_HTTP_H_ */
