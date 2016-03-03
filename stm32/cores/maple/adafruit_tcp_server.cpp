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

AdafruitTCPServer::AdafruitTCPServer(void)
{
  _connect_callback    = NULL;
//  _rx_callback         = NULL;
//  _disconnect_callback = NULL;

  _tls_private_key = NULL;
  _tls_certificate = NULL;
  _tls_certlen     = 0;
}

bool AdafruitTCPServer::begin(uint16_t port)
{
  DBG_HEAP();

  _tcp_handle = malloc_named("TCPServer", TCP_SOCKET_HANDLE_SIZE);
  uint8_t  interface      = WIFI_INTERFACE_STATION;
  uint32_t this_value     = (uint32_t) this;
  bool connect_enabled    = true;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = TCP_SOCKET_HANDLE_SIZE , .p_value = _tcp_handle         },
      { .len = 1                      , .p_value = &interface          },
      { .len = 2                      , .p_value = &port               },
      { .len = 4                      , .p_value = &this_value         },
      { .len = 1                      , .p_value = &connect_enabled    },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  VERIFY( sdep_n(SDEP_CMD_TCP_LISTEN, para_count, para_arr, NULL, NULL) );

  DBG_HEAP();

  return true;
}

bool AdafruitTCPServer::accept (void)
{

}

IPAddress AdafruitTCPServer::remoteIP ( void )
{

}

uint16_t  AdafruitTCPServer::remotePort( void )
{

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

  DBG_LOCATION();

  if (p_server->_connect_callback) p_server->_connect_callback();

  return ERROR_NONE;
}
