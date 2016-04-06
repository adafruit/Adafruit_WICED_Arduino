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
#include <adafruit_sdep.h>

// Callback proxy from Featherlib
extern "C"
{
  err_t adafruit_tcpserver_connect_callback    (void* arg, void* p_tcpserver);
}

class AdafruitTCPServer : public AdafruitSDEP
{
public:
  enum { TCP_SOCKET_HANDLE_SIZE = AdafruitTCP::TCP_SOCKET_HANDLE_SIZE };
  typedef void* tcp_handle_t;
  typedef void (*tcpserver_callback_t)(void);

  AdafruitTCPServer(uint16_t port);

  void setConnectCallback    (tcpserver_callback_t fp) { _connect_callback    = fp; }

  // Server API

  virtual bool        begin    ( void );
  virtual AdafruitTCP accept   ( void );
  virtual AdafruitTCP available( void ) { return this->accept(); }
  virtual void        stop     ( void );

protected:
  tcp_handle_t _tcp_handle;
  bool _has_connect_request;
  uint16_t _port;

  tcpserver_callback_t _connect_callback;

  // TLS
  void const * _tls_private_key;
  void const * _tls_certificate;
  uint32_t     _tls_certlen;

  bool listen (bool first_time);

public:
  friend err_t adafruit_tcpserver_connect_callback   (void* socket, void* p_tcpserver);
};

#endif /* _ADAFRUIT_TCP_SERVER_H_ */
