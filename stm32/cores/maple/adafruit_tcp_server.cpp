/**************************************************************************/
/*!
    @file     adafruit_tcp_server.cpp
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

#include "adafruit_feather.h"
#include "adafruit_tcp_server.h"

AdafruitTCPServer::AdafruitTCPServer(uint16_t port)
{
  _tcp_handle          = NULL;
  _port                = port;
  _has_connect_request = false;
  _connect_callback    = NULL;

  _tls_private_key     = NULL;
  _tls_certificate     = NULL;
  _tls_certlen         = 0;
}
/******************************************************************************/
/*!
    @brief Call listen for the first time
*/
/******************************************************************************/
bool AdafruitTCPServer::begin(void)
{
  return listen(true);
}

/******************************************************************************/
/*!
    @brief
    @para first_time whether this is the first time listen is called
*/
/******************************************************************************/
bool AdafruitTCPServer::listen(bool first_time)
{
  DBG_HEAP();
  uint8_t  interface  = WIFI_INTERFACE_STATION;
  uint32_t this_value = (uint32_t) this;

  _tcp_handle = malloc_named("TCP Server", TCP_SOCKET_HANDLE_SIZE);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = TCP_SOCKET_HANDLE_SIZE , .p_value = _tcp_handle },

      { .len = 1 , .p_value = &interface  },
      { .len = 2 , .p_value = &_port      },
      { .len = 4 , .p_value = &this_value },
      { .len = 1 , .p_value = &first_time },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  if( !sdep_n(SDEP_CMD_TCP_LISTEN, para_count, para_arr, NULL, NULL) )
  {
    free_named("TCP Server", _tcp_handle);
    _tcp_handle = NULL;
    return false;
  }
  DBG_HEAP();

  return true;
}

AdafruitTCP AdafruitTCPServer::accept (void)
{
  if ( !_has_connect_request ) return AdafruitTCP();

  // Accept the client connect request
  if( !sdep(SDEP_CMD_TCP_ACCEPT, 4, &_tcp_handle, NULL, NULL) ) return AdafruitTCP();

  // Create Client and get its IP/Port info
  AdafruitTCP accepted_client = AdafruitTCP(_tcp_handle);
  (void) accepted_client.remotePort();

  _has_connect_request = false;

  // Relisten to continue to serve other clients
  (void) listen(false); // relisten : first_time = false

  return accepted_client;
}

void AdafruitTCPServer::stop ( void )
{
  DBG_HEAP();

  if ( _tcp_handle == NULL) return;

  sdep(SDEP_CMD_TCP_DISCONNECT, 4, &_tcp_handle, NULL, NULL);
  free_named("TCP Server", _tcp_handle);

  DBG_HEAP();
}

//--------------------------------------------------------------------+
// Callbacks
//--------------------------------------------------------------------+

/******************************************************************************/
/*!
    @brief This callback is invoked when there is a new connection request
*/
/******************************************************************************/
err_t adafruit_tcpserver_connect_callback(void* socket, void* p_tcpserver)
{
  AdafruitTCPServer* p_server = (AdafruitTCPServer*) p_tcpserver;

  p_server->_has_connect_request = true;
  if (p_server->_connect_callback) p_server->_connect_callback();

  return ERROR_NONE;
}
