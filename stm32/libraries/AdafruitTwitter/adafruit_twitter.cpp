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

#define TWITTER_UPDATE_JSON   "/1.1/statuses/update.json"
//#define TWITTER_UPDATE_JSON   "/1/statuses/update.json"
#define TWITTER_API_HOST      "api.twitter.com"
#define TWITTER_API_PORT      443
#define TWITTER_AGENT         "curl/7.45.0"

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

bool AdafruitTwitter::tweet(char const* status)
{
  char buffer[256+256];
  char* buffer1 = buffer;
  char* buffer2 = buffer+256;

  //------------- OAUTH paramerters -------------//
  // Generate Auth nonce
  char nonce[32+1];
  nonce[32] = 0;
  rng_base64(nonce, 32);

  // Timestamp (UTC)
  char timestamp[20];
  sprintf(timestamp, "%u", Feather.getUtcTime());

  // OAUTH parameters
  struct {
    const char* key;
    const char* value;
  }oauth_para[] =
  {
      { "oauth_consumer_key"     , _consumer_key },
      { "oauth_nonce"            , nonce         },
      { "oauth_signature"        , NULL          },
      { "oauth_signature_method" , "HMAC-SHA1"   },
      { "oauth_timestamp"        , timestamp     },
      { "oauth_token"            , _token_access },
      { "oauth_version"          , "1.0"         },
  };
  uint8_t oauth_para_count = sizeof(oauth_para) / (2*sizeof(const char*));

  //------------- Create Signature with HMAC SHA1 -------------//
  AdafruitSHA1 sha1;
  char signature[50];

  // construct Signing Key
  sprintf(buffer1, "%s&%s", _consumer_secret, _token_secret);
  sha1.beginHMAC(buffer1);

  // POST&https://api.twitter.com/1/statuses/update.json&
  sha1.updateHMAC("POST&");
  AdafruitHTTP::urlEncode("https://" TWITTER_API_HOST TWITTER_UPDATE_JSON, buffer1, sizeof(buffer)/2);
  sha1.updateHMAC(buffer1);
  sha1.updateHMAC("&");

  // Hash Oauth parameters
  for(uint8_t i=0; i<oauth_para_count; i++)
  {
    // skip oauth_signature
    if ( oauth_para[i].value )
    {
      sprintf(buffer1, "%s=%s&", oauth_para[i].key, oauth_para[i].value);
      AdafruitHTTP::urlEncode(buffer1, buffer2, sizeof(buffer)/2);
      sha1.updateHMAC(buffer2);
    }
  }

  // Hash status, status message is double encoded
  AdafruitHTTP::urlEncode(status, buffer1, sizeof(buffer)/2);
  AdafruitHTTP::urlEncode(buffer1, buffer2, sizeof(buffer)/2);
  sha1.updateHMAC("status%3D"); // "status=" in urlencode
  sha1.updateHMAC(buffer2);

  // Stop HMAC SHA1, result is 20 bytes
  sha1.stopHMAC(buffer1);

  // convert to Base64 then urlEncode HMAC-SHA1 result
  AdafruitHTTP::base64Encode((uint8_t*)buffer1, 20, buffer2, sizeof(buffer)/2);
  AdafruitHTTP::urlEncode(buffer2, signature, sizeof(signature));

  // Update OAUTH table
  oauth_para[2].value = signature;

  //------------- Construct Authorization header -------------//
  char* p_auth = buffer;
  p_auth += sprintf(p_auth, "OAuth ");

  for(uint8_t i=0; i<oauth_para_count; i++)
  {
    if (i != 0 )
    {
      *p_auth++ = ',';
      *p_auth++ = ' ';
    }

    p_auth += sprintf(p_auth, "%s=\"%s\"", oauth_para[i].key, oauth_para[i].value);
  }

  Serial.println(buffer);

  //------------- Send HTTP request -------------//
  AdafruitHTTP _http;
  _http.err_actions(_err_print, _err_halt);
  _http.verbose(true);

  _http.connectSSL(TWITTER_API_HOST, TWITTER_API_PORT);

  // Setup the HTTP request with any required header entries
  _http.addHeader("Accept", "*/*");
  _http.addHeader("Connection", "close");
  _http.addHeader("User-Agent", TWITTER_AGENT);
  _http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  _http.addHeader("Authorization", buffer);
  _http.post(TWITTER_UPDATE_JSON, "status", status);

  _http.disconnect();
  _http.clearHeaders();
}

void AdafruitTwitter::reset(void)
{
  _consumer_key = _consumer_secret = NULL;
  _token_access = _token_secret    = NULL ;
}
