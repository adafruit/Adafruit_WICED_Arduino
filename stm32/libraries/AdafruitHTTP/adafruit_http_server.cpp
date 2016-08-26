/**************************************************************************/
/*!
    @file     adafruit_http_server.cpp
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

#include "adafruit_http_server.h"

// Callback proxy from Featherlib
extern "C"
{
  int32_t adafruit_httpserver_url_generator_callback(const char* url, const char* query, void* response_stream, void* args[], void* http_data );
}


/******************************************************************************/
/**
 * Constructors
 * @param max_pages
 */
/******************************************************************************/
AdafruitHTTPServer::AdafruitHTTPServer(uint8_t max_pages, uint8_t interface) : _page_max(max_pages)
{
  _interface = interface;

  // extra zero page as terminator
  uint32_t size = (max_pages+1)*sizeof(HTTPPage);
  _pages = (HTTPPage*) malloc(size);
  memclr(_pages, size);

  _page_count = 0;
}

AdafruitHTTPServer::~AdafruitHTTPServer()
{
  free(_pages);
  _pages = NULL;
}

void AdafruitHTTPServer::clear(void)
{

}

void AdafruitHTTPServer::addPages(HTTPPage const * http_pages, uint8_t count)
{
  for(uint8_t i=0;
      i<count && _page_count < _page_max;
      i++, _page_count++)
  {
    memcpy(_pages+_page_count, http_pages + i, sizeof(HTTPPage));
  }
}

void AdafruitHTTPServer::homepage(HTTPPage const* http_page)
{

}

/******************************************************************************
 * Start the HTTP Server
 * @param port          Listening port
 * @param max_clients   Max number of clients can access server simultaneously.
 *                      More clients will consume more featherlib's SRAM
 * @param stacksize     HTTP Server's stack size in bytes. HTTP Serverrun on its
 *                      own thread/task. Specific application may want to
 *                      increase/decrease stack size (default: HTPPSREVER_STACKSIZE_DEFAULT)
 *
 * @return true if successful, false otherwise
 *****************************************************************************/
bool AdafruitHTTPServer::begin(uint16_t port, uint8_t max_clients, uint32_t stacksize)
{
  sdep_cmd_para_t para_arr[] =
  {
      { .len = 0      , .p_value = NULL      },
  };

  if ( !sdep_n(SDEP_CMD_HTTPSERVER_START, arrcount(para_arr), para_arr, NULL, NULL) )
  {
    return false;
  }

  return true;
}


void AdafruitHTTPServer::stop(void)
{

}

int AdafruitHTTPServer::url_generator_callback(const char* url, const char* query, void* response_stream, void* http_data )
{

}





//--------------------------------------------------------------------+
// Callback
//--------------------------------------------------------------------+
/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
int32_t adafruit_httpserver_url_generator_callback(const char* url, const char* query, void* response_stream, void* args[], void* http_data )
{
  AdafruitHTTPServer* p_server = (AdafruitHTTPServer*) args[1];

  if ( p_server )
  {
    p_server->url_generator_callback(url, query, response_stream, http_data);
  }
}
