/**************************************************************************/
/*!
    @file     http_common.cpp
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

#include "http_common.h"

ASSERT_STATIC(sizeof(HTTPPage) == 24);
ASSERT_STATIC(sizeof(HTTPResource) == 16);
ASSERT_STATIC(sizeof(httppage_request_t) == 12 );

const char* const http_mime_string[] =
{
    "application/x-tlv8"               ,
    "application/x-apple-binary-plist" ,
    "application/x-ns-proxy-autoconfig",
    "application/octet-stream"         ,
    "application/javascript"           ,
    "application/json"                 ,
    "application/hap+json"             ,
    "application/pairing+tlv8"         ,
    "application/hap+verify"           ,
    "text/html"                        ,
    "text/plain"                       ,
    "text/event-stream"                ,
    "text/css"                         ,
    "image/png"                        ,
    "image/gif"                        ,
    "image/vnd.microsoft.icon"         ,
    "*/*"                              , // This must always be the last mimne
};

//--------------------------------------------------------------------+
// HTTPPage Constructor
//--------------------------------------------------------------------+
HTTPPage::HTTPPage(const char* page_url, const char* redirect_url)
{
  url          = page_url;
  _mime_type   = http_mime_string[HTTP_MIME_TEXT_HTML];
  _type        = HTTPPAGE_TYPE_REDIRECT;

  _static.data = redirect_url;
  _static.len  = strlen(redirect_url);
}

HTTPPage::HTTPPage(const char* page_url, HTTPMimeType mime_type, const uint8_t* static_page, uint32_t len)
{
  url          = page_url;
  _mime_type   = http_mime_string[mime_type];
  _type        = HTTPPAGE_TYPE_STATIC;

  _static.data = static_page;
  _static.len  = len;
}

HTTPPage::HTTPPage(const char* page_url, HTTPMimeType mime_type, const char* static_or_filepath, uint32_t offset)
{
  url          = page_url;
  _mime_type   = http_mime_string[mime_type];

  // Starting with '/' is filepath to SFLASH
  if ( static_or_filepath[0] == '/' )
  {
    _type = HTTPPAGE_TYPE_SFLASH_FILE;
    _file.path   = static_or_filepath;
    _file.offset = offset;
  }else
  {
    (void) offset;

    _type = HTTPPAGE_TYPE_STATIC;
    _static.data = (const uint8_t*) static_or_filepath;
    _static.len  = strlen(static_or_filepath);
  }
}

HTTPPage::HTTPPage(const char* page_url, HTTPMimeType mime_type, httppage_generator_t page_generator)
{
  url        = page_url;
  _mime_type = http_mime_string[mime_type];
  _type      = HTTPPAGE_TYPE_DYNAMIC;

  _dynamic.featherlib_cb  = (void*) adafruit_httpserver_url_generator_callback;
  _dynamic.page_generator = page_generator;
  _dynamic.this_ptr       = NULL;
}

HTTPPage::HTTPPage(const char* page_url, HTTPMimeType mime_type, HTTPResource const* p_resource)
{
  url        = page_url;
  _mime_type = http_mime_string[mime_type];
  _type      = HTTPPAGE_TYPE_RESOURCE;

  _resource  = p_resource;
}
