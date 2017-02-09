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

/******************************************************************************/
/**
 * Constructors
 * @param max_pages
 */
/******************************************************************************/
AdafruitHTTPServer::AdafruitHTTPServer(uint8_t max_pages, uint8_t interface) : _page_max(max_pages)
{
  _handle    = NULL;
  _interface = interface;

  // extra zero page as terminator
  uint32_t size = (_page_max+1)*sizeof(HTTPPage);
  _pages = (HTTPPage*) malloc(size);
  memclr(_pages, size);

  _page_count = 0;
}

/******************************************************************************/
/**
 * Destructor
 */
/******************************************************************************/
AdafruitHTTPServer::~AdafruitHTTPServer()
{
  this->stop();

  free(_pages);
  _pages = NULL;
}

void AdafruitHTTPServer::addPages(HTTPPage const * http_pages, uint8_t count)
{
  for(uint8_t i=0;
      i<count && _page_count < _page_max;
      i++, _page_count++)
  {
    _pages[_page_count] = http_pages[i];

    // add this pointer if page is dyanmic
    if ( http_pages[i]._type == HTTPPAGE_TYPE_DYNAMIC )
    {
      _pages[_page_count]._dynamic.this_ptr = (void*) this;
    }
  }
}

/******************************************************************************
 * Start the HTTP Server
 * @param port          Listening port
 * @param max_clients   Max number of clients can access server simultaneously.
 *                      More clients will consume more featherlib's SRAM
 * @param stacksize     HTTP Server's stack size in bytes. HTTP Server run on its
 *                      own thread/task as daemon. Specific application may want to
 *                      increase/decrease stack size (default: HTPPSREVER_STACKSIZE_DEFAULT)
 *
 *                      SDEP parameters
 *                      0. Handle buffer
 *                      1. Interface (STA, SoftAP)
 *                      2. Listenning Port
 *                      3. Max Clients
 *                      4. HTTP Daemon Stack Size
 *                      5. HTTP Pages
 *
 * @return true if successful, false otherwise
 *****************************************************************************/
bool AdafruitHTTPServer::begin(uint16_t port, uint8_t max_clients, uint32_t stacksize)
{
  // Check if there is SFLASH file and SpiFlash module is not included
  for(uint8_t i=0; i<_page_count; i++)
  {
    if ( _pages[i]._type == HTTPPAGE_TYPE_SFLASH_FILE )
    {
      if (!adafruit_arduino.httpserver_sflash_file_callback) return false;
    }
  }

  DBG_HEAP();
  _handle = malloc_named("HTTPServer Handle", HTTPSERVER_HANDLE_SIZE);
  VERIFY( _handle != NULL );

  sdep_cmd_para_t para_arr[] =
  {
      { .len = HTTPSERVER_HANDLE_SIZE, .p_value = _handle        },

      { .len = 1, .p_value = &_interface    },
      { .len = 2, .p_value = &port          },
      { .len = 1, .p_value = &max_clients   },
      { .len = 4, .p_value = &stacksize     },

      { .len = (_page_max+1)*sizeof(HTTPPage), .p_value = _pages },

      { .len = 4, .p_value = (void*) _connect_cb   },
      { .len = 4, .p_value = (void*)_disconnect_cb },
  };

  if ( !sdep_n(SDEP_CMD_HTTPSERVER_START, arrcount(para_arr), para_arr, NULL, NULL) )
  {
    free_named("HTTPServer Handle", _handle);
    return false;
  }

  DBG_HEAP();

  return true;
}

void AdafruitHTTPServer::setConnectCallback(httpserver_callback_t fp)
{
  _connect_cb = fp;
}

void AdafruitHTTPServer::setDisconnectCallback(httpserver_callback_t fp)
{
  _disconnect_cb = fp;
}

bool AdafruitHTTPServer::started(void)
{
  return _handle != NULL;
}

int AdafruitHTTPServer::clientNum(void)
{
  uint32_t count = 0;
  sdep(SDEP_CMD_HTTPSERVER_CLIENTNUM, 4, &_handle, NULL, &count);

  return (int) count;
}


void AdafruitHTTPServer::stop(void)
{
  if ( _handle )
  {
    sdep(SDEP_CMD_HTTPSERVER_STOP, 4, &_handle, NULL, NULL);

    free_named("HTTPServer Handle", _handle);
    _handle = NULL;
  }
}

void AdafruitHTTPServer::url_generator_callback(const char* url, const char* query, void* response_stream, httppage_generator_t generator_cb, httppage_request_t* http_data)
{
  _resp_stream = response_stream;

  if (generator_cb)
  {
    generator_cb(url, query, http_data);
  }

  _resp_stream = NULL;
}


size_t AdafruitHTTPServer::write(uint8_t ch)
{
  return this->write(&ch, 1);
}

size_t AdafruitHTTPServer::write(const uint8_t *buffer, size_t size)
{
  if (_resp_stream == NULL) return 0;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4   , .p_value = _resp_stream },
      { .len = size, .p_value = buffer       },
  };

  sdep_n(SDEP_CMD_HTTPSERVER_RESP_WRITE, arrcount(para_arr), para_arr, NULL, &size);

  return size;
}

/******************************************************************************/
/*!
    @brief Callback, invoked by Featherlib when client accessing a dynamic page
*/
/******************************************************************************/
int32_t adafruit_httpserver_url_generator_callback(const char* url, const char* query, void* response_stream, void* args[], void* http_data )
{
  AdafruitHTTPServer* p_server = (AdafruitHTTPServer*) args[1];

  if ( p_server )
  {
    p_server->url_generator_callback(url, query, response_stream, (httppage_generator_t) args[0], (httppage_request_t*) http_data);
  }

  return 0;
}

