/**************************************************************************/
/*!
    @file     adafruit_http_server.h
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

#ifndef _ADAFRUIT_HTTP_SERVER_H_
#define _ADAFRUIT_HTTP_SERVER_H_

#include <Arduino.h>
#include <adafruit_feather.h>
#include "http_common.h"

#define HTTPSERVER_STACKSIZE_DEFAULT    (3*1024)

class AdafruitHTTPServer : public AdafruitSDEP, public Print
{
public:
  typedef void (*httpserver_callback_t)(uint32_t ip, uint16_t port);

  enum { HTTPSERVER_HANDLE_SIZE = 576 }; // sizeof(wiced_http_server_t)

  AdafruitHTTPServer(uint8_t max_pages, uint8_t interface = WIFI_INTERFACE_STATION);
  virtual ~AdafruitHTTPServer();

  uint8_t  interface ( void ) { return _interface; }

  void addPages(HTTPPage const* http_pages, uint8_t count = 1);

  bool begin(uint16_t port, uint8_t max_clients, uint32_t stacksize = HTTPSERVER_STACKSIZE_DEFAULT);
  void stop(void);

  void setConnectCallback(httpserver_callback_t fp);
  void setDisconnectCallback(httpserver_callback_t fp);

  bool started(void);
  int  clientNum(void);

  // Print Interface
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buffer, size_t size);
  using Print::write;

  // call from featherlib only
  void url_generator_callback(const char* url, const char* query, void* response_stream, httppage_generator_t generator_cb, httppage_request_t* http_data );

protected:
  void*         _handle;

  HTTPPage*     _pages;
  uint8_t const _page_max;
  uint8_t       _page_count;

  uint8_t       _interface;

  void*         _resp_stream;

  httpserver_callback_t _connect_cb;
  httpserver_callback_t _disconnect_cb;

  void clear(void);
};

#endif /* _ADAFRUIT_HTTP_SERVER_H_ */
