/**************************************************************************/
/*!
    @file     adafruit_twitter.cpp
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

#include "adafruit_twitter.h"
#include "rng.h"
#include "adafruit_sha1.h"

/* Implement using these guide
 * - REEST API https://dev.twitter.com/rest/public
 * - OUATH https://dev.twitter.com/oauth/overview
 * - Authorize request https://dev.twitter.com/oauth/overview/authorizing-requests
 */

#define TWITTER_API_HOST                     "api.twitter.com"
#define TWITTER_API_PORT                     443
#define TWITTER_AGENT                        "curl/7.45.0"

#define TWITTER_JSON_UPDATE                  "/1.1/statuses/update.json"
#define TWITTER_JSON_DIRECTMESSAGE_NEW       "/1.1/direct_messages/new.json"

#define URLENCODE_EQUAL                      "%3D"
#define URLENCODE_AMPERSAND                  "%26"

#define TWITTER_DEBUG                        0

#define TWITTER_OAUTH_SIGNATURE_MAXLEN       50
#define TWITTER_OAUTH_AUTHORIZATION_MAXLEN   512

/**
 * Constructor
 */
AdafruitTwitter::AdafruitTwitter(void)
{
  this->reset();
}

bool AdafruitTwitter::begin(char const* consumer_key, char const* consumer_secret, char const* token_access, char const * token_secret)
{
  _consumer_key    = consumer_key;
  _consumer_secret = consumer_secret;
  _token_access    = token_access;
  _token_secret    = token_secret;

#if defined(DEBUG_TWITTER_CONSUMER_KEY) && defined(DEBUG_TWITTER_CONSUMER_SECRET) && defined(DEBUG_TWITTER_TOKEN_ACCESS) && defined(DEBUG_TWITTER_TOKEN_SECRET)
  _consumer_key    = DEBUG_TWITTER_CONSUMER_KEY;
  _consumer_secret = DEBUG_TWITTER_CONSUMER_SECRET;
  _token_access    = DEBUG_TWITTER_TOKEN_ACCESS;
  _token_secret    = DEBUG_TWITTER_TOKEN_SECRET;
#endif
}

bool AdafruitTwitter::stop(void)
{
  this->reset();
}


void AdafruitTwitter::create_oauth_signature(char signature[], const char* http_method, const char* base_url,
                                             char const* oauth_para[][2]   , uint8_t oauth_count,
                                             char const* contents_para[][2], uint8_t contents_count)
{
  char buffer1[256];
  char buffer2[256];

  AdafruitSHA1 sha1;

  // construct Signing Key
  sprintf(buffer1, "%s&%s", _consumer_secret, _token_secret);
  sha1.beginHMAC(buffer1);

  // POST&https://api.twitter.com/1/statuses/update.json&
  sha1.updateHMAC(http_method);
  sha1.updateHMAC("&");
  AdafruitHTTP::urlEncode(base_url, buffer1, sizeof(buffer1));
  sha1.updateHMAC(buffer1);
  sha1.updateHMAC("&");

  // HMAC Oauth parameters (ASSUME key and values are already in urlencoded)
  for(uint8_t i=0; i<oauth_count; i++)
  {
    char const* key   = oauth_para[i][0];
    char const* value = oauth_para[i][1];

    // skip oauth_signature
    if ( key && value )
    {
      // urlencoded(key1=value&key2=value2)
      sha1.updateHMAC(key);
      sha1.updateHMAC(URLENCODE_EQUAL);
      sha1.updateHMAC(value);
      sha1.updateHMAC(URLENCODE_AMPERSAND);
    }
  }

  // HMAC data contents (ASSUME key are already in urlencoded)
  // main data's value are double urlencoded (since it is urlencoded when passing to HTTP Request)
  for(uint8_t i=0; i<contents_count; i++)
  {
    char const* key   = contents_para[i][0];
    char const* value = contents_para[i][1];

    if ( key && value )
    {
      sha1.updateHMAC(key);
      sha1.updateHMAC(URLENCODE_EQUAL);

      // double urlencoded
      AdafruitHTTP::urlEncode(value  , buffer1, sizeof(buffer1));
      AdafruitHTTP::urlEncode(buffer1, buffer2, sizeof(buffer2));
      sha1.updateHMAC(buffer2);

      if (i != contents_count-1) sha1.updateHMAC(URLENCODE_AMPERSAND);
    }
  }

  // Stop HMAC SHA1, result is 20 bytes
  sha1.stopHMAC(buffer1);

  // convert to Base64 then urlEncode HMAC-SHA1 result
  AdafruitHTTP::base64Encode((uint8_t*)buffer1, 20, buffer2, sizeof(buffer2));
  AdafruitHTTP::urlEncode(buffer2, signature, TWITTER_OAUTH_SIGNATURE_MAXLEN);
}


void AdafruitTwitter::generate_oauth_authorization(char authorization[], const char* http_method, const char* base_url,
                                                   char const* contents_para[][2], uint8_t contents_count)
{
  //------------- Generate Oauth nonce & timestamp -------------//
  // Generate Auth nonce
  char nonce[32+1];
  rng_base64(nonce, 32);
  nonce[32] = 0;

  // Timestamp (UTC)
  char timestamp[20];
  sprintf(timestamp, "%u", Feather.getUtcTime());

#if TWITTER_DEBUG // test code
  strcpy(timestamp, "1467911727");
  strcpy(nonce, "fa5a73d8017f88c108bc4a1a6afc0e89");
#endif

  //------------- OAUTH parameter & Data contents -------------//
  // OAUTH parameters: ASSUME key and values are already in urlencoded
  char const* oauth_para[][2] =
  {
      { "oauth_consumer_key"     , _consumer_key },
      { "oauth_nonce"            , nonce         },
      { "oauth_signature"        , NULL          },
      { "oauth_signature_method" , "HMAC-SHA1"   },
      { "oauth_timestamp"        , timestamp     },
      { "oauth_token"            , _token_access },
      { "oauth_version"          , "1.0"         },
  };
  uint8_t oauth_count = sizeof(oauth_para) / sizeof(oauth_para[0]);

  //------------- Create Signature with HMAC SHA1 -------------//
  char signature[TWITTER_OAUTH_SIGNATURE_MAXLEN];
  create_oauth_signature(signature, http_method, base_url, oauth_para, oauth_count, contents_para, contents_count);

  // Update OAUTH table
  oauth_para[2][1] = signature;

  //------------- Construct Authorization header -------------//
  // format: key1="value1", key2="value2" ...
  authorization += sprintf(authorization, "OAuth ");

  for(uint8_t i=0; i<oauth_count; i++)
  {
    char const* key   = oauth_para[i][0];
    char const* value = oauth_para[i][1];

    if (i != 0 )
    {
      *authorization++ = ',';
      *authorization++ = ' ';
    }

    authorization += sprintf(authorization, "%s=\"%s\"", key, value);
  }
}


bool AdafruitTwitter::send_request(const char* http_method, const char* json_api, const char* authorization, char const* contents_para[][2], uint8_t contents_count)
{
  //------------- Send HTTP request -------------//
  AdafruitHTTP _http;
  _http.err_actions(_err_print, _err_halt);
//  _http.verbose(true);

  _http.connectSSL(TWITTER_API_HOST, TWITTER_API_PORT);

  // Setup the HTTP request with any required header entries
  _http.addHeader("Accept", "*/*");
  _http.addHeader("Connection", "close");
  _http.addHeader("User-Agent", TWITTER_AGENT);
  _http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  _http.addHeader("Authorization", authorization);

  if ( strcmp(HTTP_METHOD_POST, http_method) == 0 )
  {
    _http.post(json_api, contents_para, contents_count);
  }else
  {

  }

  _http.stop();

  return true;
}


bool AdafruitTwitter::sendDirectMessage(char const* screen_name, char const* text)
{
  // Data contents: ASSUME key are already in urlencoded
  char const* contents_para[][2] =
  {
      { "screen_name" , screen_name },
      { "text"        , text        },
  };
  uint8_t contents_count = sizeof(contents_para) / sizeof(contents_para[0]);

#ifdef DEBUG_TWITTER_DM_SCREENNAME
  contents_para[0][1] = DEBUG_TWITTER_DM_SCREENNAME;
#endif

  char authorization[512];
  generate_oauth_authorization(authorization, HTTP_METHOD_POST, "https://" TWITTER_API_HOST TWITTER_JSON_DIRECTMESSAGE_NEW,
                               contents_para, contents_count);

#if TWITTER_DEBUG
  Serial.println();
  Serial.println(authorization);
#else
  // Send HTTP request
  send_request(HTTP_METHOD_POST, TWITTER_JSON_DIRECTMESSAGE_NEW, authorization, contents_para, contents_count);
#endif

  return true;
}

bool AdafruitTwitter::tweet(char const* status)
{
  // Data contents: ASSUME key are already in urlencoded
  char const* contents_para[][2] =
  {
      { "status"     , status }
  };
  uint8_t contents_count = sizeof(contents_para) / sizeof(contents_para[0]);

  char authorization[512];
  generate_oauth_authorization(authorization, HTTP_METHOD_POST, "https://" TWITTER_API_HOST TWITTER_JSON_UPDATE,
                               contents_para, contents_count);
#if TWITTER_DEBUG
  Serial.println();
  Serial.println(authorization);
#else
  // Send HTTP request
  send_request(HTTP_METHOD_POST, TWITTER_JSON_UPDATE, authorization, contents_para, contents_count);
#endif

  return true;
}

void AdafruitTwitter::reset(void)
{
  _consumer_key = _consumer_secret = NULL;
  _token_access = _token_secret    = NULL ;
}
