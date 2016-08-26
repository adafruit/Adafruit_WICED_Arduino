/**************************************************************************/
/*!
    @file     http_common.h
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

#ifndef _HTTP_COMMON_H_
#define _HTTP_COMMON_H_

#include <Arduino.h>

enum HTTPPageType
{
  HTTPPAGE_TYPE_STATIC =0,
  HTTPPAGE_TYPE_DYNAMIC,
  HTTPPAGE_TYPE_RESOUCE,
};


enum HTTPMimeType
{
  HTTP_MIME_TLV = 0                , // "application/x-tlv8"
  HTTP_MIME_APPLE_BINARY_PLIST     , // "application/x-apple-binary-plist"
  HTTP_MIME_APPLE_PROXY_AUTOCONFIG , // "application/x-ns-proxy-autoconfig"
  HTTP_MIME_BINARY_DATA            , // "application/octet-stream"
  HTTP_MIME_JAVASCRIPT             , // "application/javascript"
  HTTP_MIME_JSON                   , // "application/json"
  HTTP_MIME_HAP_PAIRING            , // "application/pairing+tlv8"
  HTTP_MIME_HAP_JSON               , // "application/hap+json"
  HTTP_MIME_HAP_VERIFY             , // "application/hap+verify"
  HTTP_MIME_TEXT_HTML              , // "text/html"
  HTTP_MIME_TEXT_PLAIN             , // "text/plain"
  HTTP_MIME_TEXT_EVENT_STREAM      , // "text/event-stream"
  HTTP_MIME_TEXT_CSS               , // "text/css"
  HTTP_MIME_IMAGE_GIF              , // "image/gif"
  HTTP_MIME_IMAGE_PNG              , // "image/png"
  HTTP_MIME_IMAGE_MICROSOFT        , // "image/vnd.microsoft.icon"
  HTTP_MIME_ALL                    , // "*/*" This must always be the last mimne
};

typedef void (*httppage_generator_t) (const char* url, const char* query, void* http_request);

// This struct's layout must matches with wiced_http_page_t in Featherlib
struct HTTPPage
{
  const char * _url;
  const char * _mime_type;
  uint32_t _type;

  // url_content
  union
  {
    struct {
      const void*          featherlib_cb;     // Featherlib internal used only
      httppage_generator_t page_generator;
      void*                this_ptr;
    }_dynamic;

    struct {
      const void* data;
      uint32_t len;
    }_static;
  };

  HTTPPage(const char* url, HTTPMimeType mime_type, const uint8_t* static_page, uint32_t len);
  HTTPPage(const char* url, HTTPMimeType mime_type, const char* static_page);
  HTTPPage(const char* url, HTTPMimeType mime_type, httppage_generator_t page_generator);
};


#endif /* _HTTP_COMMON_H_ */
