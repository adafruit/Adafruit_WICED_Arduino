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

#include "adafruit_tcp_server.h"

bool AdafruitTCPServer::begin(uint16_t port)
{
  DBG_HEAP();

  uint8_t  interface      = WIFI_INTERFACE_STATION;

  uint32_t this_value     = (uint32_t) this;
  bool connect_enabled    = true;
  bool rx_enabled         = true;
  bool disconnect_enabled = true;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &_tcpserver_handle  },
      { .len = 1, .p_value = &interface          },
      { .len = 2, .p_value = &port               },
      { .len = 1, .p_value = &_max_clients       },
      { .len = 0, .p_value = NULL                },
      { .len = 4, .p_value = &this_value         },
      { .len = 1, .p_value = &connect_enabled    },
      { .len = 1, .p_value = &rx_enabled         },
      { .len = 1, .p_value = &disconnect_enabled },
      { .len = 0, .p_value = NULL                },
      { .len = 0, .p_value = NULL                },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  VERIFY( sdep_n(SDEP_CMD_TCP_LISTEN, para_count, para_arr, NULL, NULL) );

  DBG_HEAP();

  return true;
}

bool AdafruitTCPServer::accept (uint32_t timeout)
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
err_t adafruit_tcpserver_connect_callback(void* socket, void* p_tcp)
{
  AdafruitTCPServer* pTCP = (AdafruitTCPServer*) p_tcp;

  DBG_LOCATION();

  // Integrity check
//  if ( *((uint32_t*) pTCP->_tcp_handle) == ((uint32_t) socket) )
//  {
//    if (pTCP->_connect_callback) pTCP->_connect_callback();
//  }

  return ERROR_NONE;
}

/******************************************************************************/
/*!
    @brief This callback is invoked when there is data received
*/
/******************************************************************************/
err_t adafruit_tcpserver_receive_callback(void* socket, void* p_tcp)
{
  AdafruitTCPServer* pTCP = (AdafruitTCPServer*) p_tcp;

  DBG_LOCATION();

  // Integrity check
//  if ( *((uint32_t*) pTCP->_tcp_handle) == ((uint32_t) socket) )
//  {
//    if (pTCP->_rx_callback) pTCP->_rx_callback();
//  }

  return ERROR_NONE;
}

/******************************************************************************/
/*!
    @brief This callback is invoked when tcp is disconnected
*/
/******************************************************************************/
err_t adafruit_tcpserver_disconnect_callback(void* socket, void* p_tcp)
{
  AdafruitTCPServer* pTCP = (AdafruitTCPServer*) p_tcp;

  DBG_LOCATION();
  // Integrity check
//  if ( *((uint32_t*) pTCP->_tcp_handle) == ((uint32_t) socket) )
//  {
//    // TODO set connected as false ???
//    if (pTCP->_disconnect_callback) pTCP->_disconnect_callback();
//  }

  return ERROR_NONE;
}

