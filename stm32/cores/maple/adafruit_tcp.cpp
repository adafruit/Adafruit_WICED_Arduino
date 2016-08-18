/**************************************************************************/
/*!
    @file     adafruit_tcp.cpp
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

#include "adafruit_feather.h"
#include "adafruit_tcp.h"

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
void AdafruitTCP::reset()
{
  _tcp_handle          = NULL;

  _tls_verification    = true;
  _tls_context         = NULL;

  _tls_identity        = NULL;
  _tls_private_key     = NULL;
  _tls_local_cert      = NULL;
  _tls_local_certlen   = 0;

  _bytesRead           = 0;
  _remote_ip           = 0;
  _remote_port         = 0;
  _packet_buffering    = false;
  _verbose             = false;

  _timeout             = ADAFRUIT_TCP_TIMEOUT;
  _rx_callback         = NULL;
  _disconnect_callback = NULL;
}

/******************************************************************************/
/*!
    @brief Constructor
*/
/******************************************************************************/
AdafruitTCP::AdafruitTCP(void)
{
  this->reset();
}

/******************************************************************************/
/*!
    @brief Constructor from an tcp handle
*/
/******************************************************************************/
AdafruitTCP::AdafruitTCP ( tcp_handle_t handle )
{
  this->reset();
  _tcp_handle = handle;
}

/******************************************************************************/
/*!
    @brief Destructor
*/
/******************************************************************************/
AdafruitTCP::~AdafruitTCP()
{
  // cannot call stop() since AdafruitTCPServer accept() won't be able to
  // return AdafruitTCP instance
}

/******************************************************************************/
/*!
    @brief Internal function
*/
/******************************************************************************/
bool AdafruitTCP::connect_internal ( uint8_t interface, uint32_t ipv4, uint16_t port, uint8_t is_tls)
{
  DBG_HEAP();

  uint8_t  tls_option = (is_tls && _tls_verification) ? TLS_VERIFICATION_REQUIRED : TLS_NO_VERIFICATION;
  _remote_ip          = ipv4;
  _remote_port        = port;

  _tcp_handle = malloc_named("TCP Client", TCP_SOCKET_HANDLE_SIZE);
  VERIFY( _tcp_handle != NULL );

  if (is_tls)
  {
    _tls_context = malloc_named("TCP TLS Context", TCP_TLS_CONTEXT_SIZE);
    VERIFY(_tls_context != NULL);

    if (_tls_private_key && _tls_local_cert )
    {
      _tls_identity = malloc_named("TCP TLS Identity", TCP_TLS_IDENTITY_SIZE);
      VERIFY(_tls_identity != NULL);
    }
  }

  sdep_cmd_para_t para_arr[] =
  {
      { .len = TCP_SOCKET_HANDLE_SIZE , .p_value = _tcp_handle       },

      { .len = 1                       , .p_value = &interface       },
      { .len = 4                       , .p_value = &_remote_ip      },
      { .len = 2                       , .p_value = &_remote_port    },
      { .len = 4                       , .p_value = &_timeout        },
      { .len = 1                       , .p_value = &is_tls          },
      { .len = 1                       , .p_value = &tls_option      },

      { .len = TCP_TLS_CONTEXT_SIZE    , .p_value = _tls_context     },
      { .len = TCP_TLS_IDENTITY_SIZE   , .p_value = _tls_identity    },
      { .len = _tls_private_key ?
           strlen(_tls_private_key) : 0, .p_value = _tls_private_key },
      { .len = _tls_local_certlen      , .p_value = _tls_local_cert  },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  if ( !sdep_n(SDEP_CMD_TCP_CONNECT, para_count, para_arr, NULL, NULL) )
  {
    free_named("TCP Client", _tcp_handle);
    if ( _tls_context ) free_named("TCP TLS Context" , _tls_context);
    if ( _tls_identity) free_named("TCP TLS Identity", _tls_identity);

    _tcp_handle = _tls_context = _tls_identity = NULL;

    return false;
  }

  this->install_callback();

  DBG_HEAP();

  return true;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
int AdafruitTCP::connect(IPAddress ip, uint16_t port)
{
  if ( !Feather.connected() ) return 0;
  return connect_internal(WIFI_INTERFACE_STATION, (uint32_t) ip, port, false);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
int AdafruitTCP::connect(const char* host, uint16_t port)
{
  IPAddress ip;
  VERIFY( Feather.hostByName(host, ip) );
  return this->connect(ip, port);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
int AdafruitTCP::connectSSL(IPAddress ip, uint16_t port)
{
  if ( !Feather.connected() ) return 0;

  // Only need to init Root CA if verification is enabled
  if ( _tls_verification )
  {
    // Init Root CA first if not initialized
    VERIFY( Feather.initRootCA() );
  }

  return connect_internal(WIFI_INTERFACE_STATION, (uint32_t) ip, port, true);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
int AdafruitTCP::connectSSL(const char* host, uint16_t port)
{
  IPAddress ip;
  VERIFY( Feather.hostByName(host, ip) );
  return this->connectSSL(ip, port);
}

bool AdafruitTCP::tlsSetIdentity(char const* private_key, uint8_t const* local_cert, uint16_t local_certlen)
{
  _tls_private_key    = private_key;
  _tls_local_cert     = local_cert;
  _tls_local_certlen  = local_certlen;

  return true;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
uint8_t AdafruitTCP::connected()
{
  // Handle not zero --> still connected
  // TODO handle disconnection
  return ( _tcp_handle != NULL ) ? 1 : 0;
}

void AdafruitTCP::get_peer_info(void)
{
  struct ATTR_PACKED {
    uint32_t remote_ip;
    uint16_t remote_port;
  } response = { 0, 0 };

  (void) sdep(SDEP_CMD_TCP_PEER_INFO, 4, &_tcp_handle, NULL, &response);

  _remote_ip   = response.remote_ip;
  _remote_port = response.remote_port;
}

IPAddress AdafruitTCP::remoteIP ( void )
{
  // only execute if remote IP is not available
  if (_remote_ip == 0) get_peer_info();
  return IPAddress(_remote_ip);
}

uint16_t  AdafruitTCP::remotePort ( void )
{
  // only execute if remote Port is not available
  if (_remote_port == 0) get_peer_info();
  return _remote_port;
}


/******************************************************************************/
/*!
    @brief  Reads a single byte from the response stream

    @return Returns -1 if no data was available, or the total number of bytes
            read so far if a byte was able to be read.
*/
/******************************************************************************/
int AdafruitTCP::read()
{
  uint8_t b;
  return ( this->read(&b, 1) > 0 ) ? b : EOF;
}

/******************************************************************************/
/*!
    @brief  Reads the specified number of bytes, placing them in 'buf'

    @return The total number of bytes read so far
*/
/******************************************************************************/
int AdafruitTCP::read(uint8_t* buf, size_t size)
{
  if ( !connected() ) return 0;

  uint16_t size16 = (uint16_t) size;
  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &_tcp_handle },
      { .len = 2, .p_value = &size16      },
      { .len = 4, .p_value = &_timeout    },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  uint16_t readlen = size16;
  VERIFY_RETURN( sdep_n(SDEP_CMD_TCP_READ, para_count, para_arr, &readlen, buf), 0);

  _bytesRead += readlen;
  return readlen;
}

/******************************************************************************/
/*!
    @brief  Writes a byte
*/
/******************************************************************************/
size_t AdafruitTCP::write( uint8_t b)
{
  return write(&b, 1);
}

/******************************************************************************/
/*!
    @brief  Writes the specified number of bytes
*/
/******************************************************************************/
size_t AdafruitTCP::write(const uint8_t* content, size_t len)
{
  if ( !connected() ) return 0;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4  , .p_value = &_tcp_handle},
      { .len = len, .p_value = content     }
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);
  
  VERIFY_RETURN( sdep_n(SDEP_CMD_TCP_WRITE, para_count, para_arr, NULL, NULL), 0);

  // if packet is not buffered --> send out immediately
  if (!_packet_buffering) this->flush();

  return len;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
void AdafruitTCP::flush()
{
  if ( !connected() ) return;

  // flush write
  sdep(SDEP_CMD_TCP_FLUSH, 4, &_tcp_handle, NULL, NULL);

  // TODO should we flush read bytes as well ??
  //while (available()) read();
}

/******************************************************************************/
/*!
    @brief  Returns the number of bytes available in the response FIFO
*/
/******************************************************************************/
int AdafruitTCP::available()
{
  if ( !connected() ) return 0;

  int32_t result = 0;
  sdep(SDEP_CMD_TCP_AVAILABLE, 4, &_tcp_handle, NULL, &result);

  return result;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
int AdafruitTCP::peek()
{
  if ( !connected() ) return EOF;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &_tcp_handle },
      { .len = 4, .p_value = &_timeout    },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  char ch = EOF;
  sdep_n(SDEP_CMD_TCP_PEEK, para_count, para_arr, NULL, &ch);

  return (int) ch;
}

/******************************************************************************/
/*!
    @brief  Passes the callback references to the lower WiFi layer
*/
/******************************************************************************/
void AdafruitTCP::install_callback(void)
{
  // connect callback is not supported yet
  bool connect_enabled    = false;
  bool rx_enabled         = (_rx_callback != NULL);
  bool disconnect_enabled = (_disconnect_callback != NULL);

  // no callbacks are enabled
  if ( !(connect_enabled || rx_enabled || disconnect_enabled) ) return;

  uint32_t this_value = (uint32_t) this;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &_tcp_handle        },
      { .len = 4, .p_value = &this_value         },
      { .len = 1, .p_value = &connect_enabled    },
      { .len = 1, .p_value = &rx_enabled         },
      { .len = 1, .p_value = &disconnect_enabled },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  // TODO check case when read bytes < size
  sdep_n(SDEP_CMD_TCP_SET_CALLBACK, para_count, para_arr, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Sets the data received callback for the user code
*/
/******************************************************************************/
void AdafruitTCP::setReceivedCallback( tcpcallback_t fp )
{
  _rx_callback = fp;
}

/******************************************************************************/
/*!
    @brief  Sets the disconnect callback for the user code
*/
/******************************************************************************/
void AdafruitTCP::setDisconnectCallback( tcpcallback_t fp )
{
  _disconnect_callback = fp;
}

/******************************************************************************/
/*!
    @brief  Closes the TCP socket and resets any counters
*/
/******************************************************************************/
void AdafruitTCP::stop()
{
  DBG_HEAP();

  if ( _tcp_handle == NULL) return;

  sdep(SDEP_CMD_TCP_DISCONNECT, 4, &_tcp_handle, NULL, NULL);

  free_named("TCP Client", _tcp_handle);
  if ( _tls_context ) free_named("TCP TLS Context" , _tls_context);
  if ( _tls_identity) free_named("TCP TLS Identity", _tls_identity);

  this->reset();

  DBG_HEAP();
}


//--------------------------------------------------------------------+
// Callbacks
//--------------------------------------------------------------------+
/******************************************************************************/
/*!
    @brief This callback is invoked when there is data received
*/
/******************************************************************************/
err_t adafruit_tcp_receive_callback(void* socket, void* p_tcp)
{
  AdafruitTCP* pTCP = (AdafruitTCP*) p_tcp;

  if (pTCP->_rx_callback) pTCP->_rx_callback();

  return ERROR_NONE;
}

/******************************************************************************/
/*!
    @brief This callback is invoked when tcp is disconnected
*/
/******************************************************************************/
err_t adafruit_tcp_disconnect_callback(void* socket, void* p_tcp)
{
  AdafruitTCP* pTCP = (AdafruitTCP*) p_tcp;

  // TODO set connected as false or call TCP::Stop() ???

  if (pTCP->_disconnect_callback) pTCP->_disconnect_callback();

  return ERROR_NONE;
}

