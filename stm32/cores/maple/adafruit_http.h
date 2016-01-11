/**************************************************************************/
/*!
    @file     adafruit_http.h
    @author   huynguyen

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

typedef enum {
  IDLE,
  REQUEST_SENT,
  HEADER_PASSED,
} httpState_t;

class AdafruitHTTP
{
private:
  httpState_t http_state;
  uint32_t    http_handle;
  uint32_t    timeout;
  uint32_t    cert_addr;
  uint8_t     isTLSEnable;
  uint32_t    byteRead;

  int (*rx_callback) (void *, void*);
  void reset();

public:
  AdafruitHTTP();

  void initCACerts(const char* ca_cert);
  void deinitCACerts();
  void enableTLS();
  void disableTLS();
  void setTimeout(uint32_t ms);
  int  sendRequest(const char* url, const char* content, uint8_t method);
  long getPacketDataLength();
  int  getResponseCode();
  int  extractHeaderValue(const char* header_name, char* value);
  int  endOfHeaderReached();
  int  skipHeader();
  int  read();
  int  read(uint8_t *buf, size_t size);
  int  available();
  void close();

  // callback
  void setReceivedCallback( int (*fp) (void*, void*) );
};

#endif
