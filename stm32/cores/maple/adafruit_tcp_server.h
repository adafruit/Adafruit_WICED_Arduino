/**************************************************************************/
/*!
    @file     adafruit_tcp_server.h
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

#ifndef _ADAFRUIT_TCP_SERVER_H_
#define _ADAFRUIT_TCP_SERVER_H_

#include <Arduino.h>
#include <Client.h>
#include <IPAddress.h>
#include "adafruit_feather.h"

// Callback proxy from Featherlib
extern "C"
{
  err_t adafruit_tcpserver_connect_callback    (void* arg, void* p_tcp);
  err_t adafruit_tcpserver_receive_callback    (void* arg, void* p_tcp);
  err_t adafruit_tcpserver_disconnect_callback (void* arg, void* p_tcp);
}

class AdafruitTCPServer : public AdafruitSDEP
{
public:
  typedef void (*tcpserver_callback_t)(void);

  AdafruitTCPServer(uint8_t max_clients)
  {
    _max_clients = max_clients;
    memclr(_tcpserver_handle, TCP_SERVER_HANDLE_SIZE);

    _connect_callback    = NULL;
    _rx_callback         = NULL;
    _disconnect_callback = NULL;
  }

  void setConnectCallback    (tcpserver_callback_t fp) { _connect_callback    = fp; }
  void setDisconnectCallback (tcpserver_callback_t fp) { _disconnect_callback = fp; }
  void setReceivedCallback   (tcpserver_callback_t fp) { _rx_callback         = fp; }

  // Server API

  virtual bool      begin      (uint16_t port);
  virtual bool      accept     (uint32_t timeout);
  virtual IPAddress remoteIP   ( void );
  virtual uint16_t  remotePort ( void );

protected:
  enum { TCP_SERVER_HANDLE_SIZE = 32 };

  uint8_t   _tcpserver_handle[TCP_SERVER_HANDLE_SIZE];
  uint8_t   _max_clients;

  tcpserver_callback_t _connect_callback;
  tcpserver_callback_t _rx_callback;
  tcpserver_callback_t _disconnect_callback;
public:
  // Callback
  friend err_t adafruit_tcpserver_connect_callback(void* socket, void* p_tcp);
  friend err_t adafruit_tcpserver_receive_callback(void* socket, void* p_tcp);
  friend err_t adafruit_tcpserver_disconnect_callback(void* socket, void* p_tcp);

};

#endif /* _ADAFRUIT_TCP_SERVER_H_ */
