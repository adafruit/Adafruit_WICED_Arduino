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
#include <Client.h>
#include <IPAddress.h>
#include "adafruit_feather.h"

extern "C"
{
  err_t adafruit_tcp_receive_callback(void* arg, void* p_tcp);
  err_t adafruit_tcp_disconnect_callback(void* arg, void* p_tcp);
}

class AdafruitTCP : public Client, public AdafruitSDEP
{
protected:
  uint32_t  _tcp_handle;
  uint32_t  _bytesRead;

  // buffer written until network packet is full ~1500 or flush() is called
  // default is false
	bool     _packet_buffering;

  // Callback signatures
  void (*rx_callback)         (AdafruitTCP* pTCP);
  void (*disconnect_callback) (AdafruitTCP* pTCP);

  void install_callback ( void );
  void reset ( void );

public:
  AdafruitTCP ( void );
  virtual ~AdafruitTCP();

  void usePacketBuffering(bool enable);

  // Client API
  virtual int     connect ( IPAddress ip, uint16_t port );
  virtual int     connect ( const char * host, uint16_t port );

  int connectSSL(IPAddress ip, uint16_t port);
	int connectSSL(const char* host, uint16_t port);

//  int connectSSL(IPAddress ip, uint16_t port, char const* common_name = NULL);
//	int connectSSL(const char* host, uint16_t port, char const* common_name = NULL);

  virtual uint8_t connected( void );
  virtual void    stop    ( void );

  virtual operator bool() { return _tcp_handle != 0; }

  // Stream API
  virtual int    read       ( void );
  virtual int    read       ( uint8_t * buf, size_t size );
  virtual size_t write      ( uint8_t );
  virtual size_t write      ( const uint8_t *content, size_t len );
  virtual int    available  ( void );
  virtual int    peek       ( void );
  virtual void   flush      ( void );

  using Print::write;

  // Set callback handlers
  void setReceivedCallback   ( void (*fp) (AdafruitTCP* pTCP));
  void setDisconnectCallback ( void (*fp) (AdafruitTCP* pTCP));

  friend err_t adafruit_tcp_receive_callback(void* socket, void* p_tcp);
  friend err_t adafruit_tcp_disconnect_callback(void* socket, void* p_tcp);
};

#endif
