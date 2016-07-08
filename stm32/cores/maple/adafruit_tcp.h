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
#include <adafruit_sdep.h>

#define ADAFRUIT_TCP_TIMEOUT  1000

// Callback proxy from Featherlib
extern "C"
{
  err_t adafruit_tcp_receive_callback    (void* arg, void* p_tcp);
  err_t adafruit_tcp_disconnect_callback (void* arg, void* p_tcp);
}

class AdafruitTCP : public Client, public AdafruitSDEP
{
public:
  enum {
    TCP_SOCKET_HANDLE_SIZE = 400 , // need only 372 (352+20) bytes, extra for reserved
    TCP_TLS_CONTEXT_SIZE   = 2600, // need 2588 bytes, extra for reserved
    TCP_TLS_IDENTITY_SIZE  = 200 , // need 184 bytes, extra for reserved
  };
  typedef void* tcp_handle_t;
  typedef void (*tcpcallback_t)(void);

  AdafruitTCP ( tcp_handle_t handle );
  AdafruitTCP ( void );
  virtual ~AdafruitTCP();

  virtual operator bool() { return _tcp_handle != NULL; }

  void     verbose                ( bool enable ) { _verbose = enable; }
  void     usePacketBuffering     ( bool enable ) { _packet_buffering = enable; }
  uint32_t getHandle              ( void        ) { return (uint32_t) _tcp_handle; }

  void     tlsRequireVerification (bool required) { _tls_verification = required; }
  bool     tlsSetIdentity         (char const* private_key, uint8_t const* local_cert, uint16_t local_certlen);

  // Client API
  virtual int       connect    ( IPAddress ip, uint16_t port );
  virtual int       connect    ( const char * host, uint16_t port );
  virtual int       connectSSL ( IPAddress ip, uint16_t port );
  virtual int       connectSSL ( const char* host, uint16_t port );

  virtual uint8_t   connected  ( void );
  virtual void      stop       ( void );
  void              disconnect ( void ) { stop(); }
  virtual IPAddress remoteIP   ( void );
  virtual uint16_t  remotePort ( void );

  // Stream API
  virtual int       read       ( void );
  virtual int       read       ( uint8_t * buf, size_t size );
  virtual size_t    write      ( uint8_t b );
  virtual size_t    write      ( const uint8_t *content, size_t len );
  virtual int       available  ( void );
  virtual int       peek       ( void );
  virtual void      flush      ( void );

  int               read       ( char* buf, size_t size) { return read((uint8_t*) buf, size); }
  uint32_t          byteRead   (void) { return _bytesRead; }


  using Print::write;

  // Set callback handlers
  void setReceivedCallback    ( tcpcallback_t fp );
  void setDisconnectCallback  ( tcpcallback_t fp );

  friend err_t adafruit_tcp_receive_callback(void* socket, void* p_tcp);
  friend err_t adafruit_tcp_disconnect_callback(void* socket, void* p_tcp);

protected:
  tcp_handle_t   _tcp_handle;

  bool           _tls_verification;

  void*          _tls_context;
  void*          _tls_identity;

  char const*    _tls_private_key;
  const uint8_t* _tls_local_cert;
  uint16_t       _tls_local_certlen;

  uint32_t     _bytesRead;

  uint32_t     _remote_ip;
  uint16_t     _remote_port;

  // If enabled, data is written to a buffer until the network packet is full
  // (~1500 bytes) or until .flush() is called
  // Default = false (buffering disabled)
  bool          _packet_buffering;

  // Callback prototypes
  tcpcallback_t _rx_callback;
  tcpcallback_t _disconnect_callback;

  bool connect_internal ( uint8_t interface, uint32_t ipv4, uint16_t port, uint8_t is_tls);
  void install_callback ( void );
  void reset            ( void );
  void get_peer_info    ( void );

private:
  bool          _verbose;
};

#endif
