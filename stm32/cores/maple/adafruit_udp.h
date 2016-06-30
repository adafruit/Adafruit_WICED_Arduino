/**************************************************************************/
/*!
    @file     adafruit_udp.h
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

#ifndef _ADAFRUIT_UDP_H_
#define _ADAFRUIT_UDP_H_

#include <Arduino.h>
#include <Udp.h>
#include "adafruit_sdep.h"

#define ADAFRUIT_UDP_TIMEOUT  200

extern "C"
{
  err_t adafruit_udp_receive_callback(void* socket, void* p_udp);
}

class AdafruitUDP : public UDP, public AdafruitSDEP
{
public:
  typedef void* udp_handle_t;
  typedef void (*udpcallback_t)(void);

  AdafruitUDP();

  void              usePacketBuffering ( bool enable ) { _packet_buffering = enable; }

  // UDP API
  virtual uint8_t   begin       ( uint16_t port );
  virtual void      stop        ( void );

  virtual int       beginPacket ( IPAddress ip, uint16_t port );
  virtual int       beginPacket ( const char *host, uint16_t port );
  virtual int       endPacket   ( void );

  virtual int       parsePacket ( void );
  virtual IPAddress remoteIP    ( void );
  virtual uint16_t  remotePort  ( void );

  // Stream API
  virtual int       read        ( void );
  virtual int       read        ( unsigned char* buffer, size_t len );
  virtual int       read        (char* buffer, size_t len )
  {
    return this->read( (unsigned char*) buffer, len );
  }
  virtual int       peek        ( void );
  virtual int       available   ( void );
  virtual void      flush       ( void );
  virtual size_t    write       ( uint8_t byte );
  virtual size_t    write       ( const uint8_t *buffer, size_t size );

  using Print::write;

  // Callbacks ( must be set before begin() )
  void  setReceivedCallback ( udpcallback_t fp );

  friend err_t adafruit_udp_receive_callback(void* socket, void* p_udp);

protected:
  uint32_t  _udp_handle;
  uint16_t  _rcvPort;
  uint32_t  _rcvIP;
  uint16_t  _sndPort;
  uint32_t  _sndIP;
  uint32_t  _bytesRead;

  // If enabled, data is written to a buffer until the network packet is full
  // (~1500 bytes) or until .flush() is called
  // Default = true (buffering enabled)
  bool      _packet_buffering;

  udpcallback_t rx_callback;

  void reset(void);
};

#endif
