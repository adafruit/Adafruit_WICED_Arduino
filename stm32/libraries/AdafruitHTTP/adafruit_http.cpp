/**************************************************************************/
/*!
    @file     adafruit_http.cpp
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2015, Adafruit Industries (adafruit.com)
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

#include "adafruit_http.h"

#define HTTP_GET      "GET"
#define HTTP_POST     "POST"
#define HTTP_VERSION  "HTTP/1.1"

AdafruitHTTP::AdafruitHTTP()
{
  _packet_buffering = true;
  _server = NULL;

  this->clearHeaders();
}

int AdafruitHTTP::connect( const char * host, uint16_t port )
{
  _server = host;
  return AdafruitTCP::connect(host, port);
}

int AdafruitHTTP::connectSSL( const char* host, uint16_t port )
{
  _server = host;
  return AdafruitTCP::connectSSL(host, port);
}


bool AdafruitHTTP::addHeader(const char* name, const char* value)
{
  if (_header_count >= ADAFRUIT_HTTP_MAX_HEADER) return false;

  _headers[_header_count].name  = name;
  _headers[_header_count].value = value;

  _header_count++;
  return true;
}

bool AdafruitHTTP::clearHeaders(void)
{
  _header_count     = 0;
  for(uint8_t i=0; i<ADAFRUIT_HTTP_MAX_HEADER; i++)
  {
    _headers[i].name = _headers[i].value = NULL;
  }
}

void AdafruitHTTP::sendHeaders(size_t content_len)
{
  for(uint8_t i=0; i<_header_count; i++)
  {
    printf("%s: %s", _headers[i].name, _headers[i].value);
    println();
  }

  // Add "Content-Length" if available
  if (content_len)
  {
    printf("Content-Length: %u", content_len);
    println();
  }

  println(); // End of header
}

bool AdafruitHTTP::get(char const * host, char const *url)
{
  printf(HTTP_GET " %s " HTTP_VERSION, url); println();
  printf("Host: %s", host); println();

  sendHeaders(0);
  flush();
}

bool AdafruitHTTP::get(char const *url)
{
  if(_server == NULL) return false;

  this->get(_server, url);
}

bool AdafruitHTTP::post(char const * host, char const *url, char const* data)
{
  printf(HTTP_POST " %s " HTTP_VERSION, url); println();
  printf("Host: %s", host); println();

  sendHeaders( strlen(data) );

  // send data
  println(data);

  flush();
}

bool AdafruitHTTP::post(char const *url, char const* data)
{
  if(_server == NULL) return false;

  this->get(url, data);
}
