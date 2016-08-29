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
#include <adafruit_urlencode.h>
#include "http_common.h"

#define ADAFRUIT_HTTP_MAX_HEADER  10

class AdafruitHTTP : public AdafruitTCP
{
protected:
  const char* _server;

  int _resp_len;
  int _resp_status;

  uint8_t _header_count;
  struct {
    const char* name;
    const char* value;
  }_headers[ADAFRUIT_HTTP_MAX_HEADER];

  void sendHeaders(size_t content_len);
  void send_keyvalues_data(const char* keyvalues[][2], uint16_t count, bool url_encode);

  bool post_internal(char const * host, char const *url, const char* keyvalues[][2], uint16_t count, bool url_encode);

private:
  bool _verbose;

public:
  AdafruitHTTP(uint8_t interface = WIFI_INTERFACE_STATION);

  void verbose(bool enable) { _verbose = enable; }

  bool addHeader(const char* name, const char* value);
  bool clearHeaders(void);

  int  readline(char* buffer, uint16_t bufsize);
  int  readline(void); // skip line
  int  readUntil(char terminator, char *buffer, uint16_t bufsize);

  //------------- GET request -------------//
  bool get(char const *host, char const *url, const char* keyvalues[][2], uint16_t count);
  bool get(char const *host, char const *url);
  bool get(char const *url, const char* keyvalues[][2], uint16_t count);
  bool get(char const *url);

  //------------- Response processing -------------//
  bool respParseHeader(void);
  int  respStatus(void);
  int  respContentLength(void);


  //------------- POST with urlencoding data -------------//
  bool post(char const *host, char const *url, const char* keyvalues[][2], uint16_t count);
  bool post(char const *host, char const *url, char const* key, char const* value);
  bool post(char const *url, const char* keyvalues[][2], uint16_t count);
  bool post(char const *url, char const* key, char const* value);

  //------------- POST without urlencoding data -------------//
  bool postWithoutURLencoded(char const *host, char const *url, const char* keyvalues[][2], uint16_t count);
  bool postWithoutURLencoded(char const *host, char const *url, char const* key, char const* value);
  bool postWithoutURLencoded(char const *url, const char* keyvalues[][2], uint16_t count);
  bool postWithoutURLencoded(char const *url, char const* key, char const* value);

  //------------- POST with Raw data, useful with binary -------------//
  bool postRaw(char const *host, char const *url, uint8_t const* raw_data, uint16_t len);
  bool postRaw(char const *host, char const *url, char const* raw_data);
  bool postRaw(char const *url, uint8_t const* raw_data, uint16_t len );
  bool postRaw(char const *url, char const* raw_data);

  //------------- TCP API -------------//
  virtual int    connect    ( const char *host, uint16_t port );
  virtual int    connectSSL ( const char *host, uint16_t port );
  virtual void   stop       ( void );
  virtual size_t write      ( uint8_t b);
  virtual size_t write      ( const uint8_t *content, size_t len );
};

#endif /* _ADAFRUIT_HTTP_H_ */
