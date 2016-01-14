/**************************************************************************/
/*!
    @file     adafruit_tcp.h
    @author   ktown

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

#ifndef _ADAFRUIT_TCP_H_
#define _ADAFRUIT_TCP_H_

#include <Arduino.h>
#include <IPAddress.h>

typedef enum {
  IDLE,
  REQUEST_SENT
} tcpState_t;

class AdafruitTCP
{
private:
  tcpState_t  tcp_state;
  uint32_t    tcp_handle;
  uint32_t    timeout;
  uint32_t    bytesRead;
  uint16_t    socket;

  int (*rx_callback) (void *, void*);
  int (*disconnect_callback) (void *, void*);
  void install_callback(void);
  void reset();

public:
  //AdafruitTCP(uint16_t socket_number);
  AdafruitTCP(void);

  void setTimeout(uint32_t ms);
  int  read();
  int  read(uint8_t *buf, size_t size);
  int  write(const char* content, uint16_t len);
  int  available();
  void close();
  void flush(void);

  bool connect(IPAddress ip, uint16_t port);
	bool connect(const char* host, uint16_t port);

  // callback
  void setReceivedCallback( int (*fp) (void*, void*) );
  void setDisconnectCallback( int (*fp) (void*, void*));
};

#endif
