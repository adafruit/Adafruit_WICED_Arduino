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
#include "adafruit_base64.h"

/* Implement using these guide
 * - REST API https://dev.twitter.com/rest/public
 * - OUATH https://dev.twitter.com/oauth/overview
 * - Authorize request https://dev.twitter.com/oauth/overview/authorizing-requests
 */

#define TWITTER_API_HOST                     "api.twitter.com"
#define TWITTER_API_PORT                     443
#define TWITTER_AGENT                        "curl/7.45.0"

#define TWITTER_JSON_UPDATE                  "/1.1/statuses/update.json"
#define TWITTER_JSON_DIRECTMESSAGE_NEW       "/1.1/direct_messages/new.json"
#define TWITTER_JSON_DIRECTMESSAGE           "/1.1/direct_messages.json"

#define URLENCODE_EQUAL                      "%3D"
#define URLENCODE_AMPERSAND                  "%26"

#define TWITTER_DEBUG                        0

#define TWITTER_OAUTH_SIGNATURE_MAXLEN       50
#define TWITTER_OAUTH_AUTHORIZATION_MAXLEN   512

static void sha1_keyvalue(AdafruitSHA1& sha1, char const* keyvalue[2], bool is_http_data);

/**
 * Constructor
 */
AdafruitTwitter::AdafruitTwitter(void)
{
  this->reset();
}

/**
 * Initialize Twitter class
 * @param consumer_key      Consumer Key
 * @param consumer_secret   Consumer Secret
 * @param token_access      Token Access
 * @param token_secret      Token Secret
 * @return  true if successful, false otherwise
 */
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

/**
 * De-initialize Twitter class
 * @return  true if successful, false otherwise
 */
bool AdafruitTwitter::stop(void)
{
  this->reset();
}

/**
 * Reset class state
 */
void AdafruitTwitter::reset(void)
{
  _consumer_key = _consumer_secret = NULL;
  _token_access = _token_secret    = NULL ;

  _dm_last_sinceid     = 0;
  _dm_rx_callback = NULL;
}

/**
 * Post status's tweet
 * @param status  Stastus text
 * @return  true if successful, false otherwise
 */
bool AdafruitTwitter::tweet(char const* status)
{
  // Data contents: ASSUME key are already in urlencoded
  char const* httpdata[][2] =
  {
      { "status"     , status }
  };
  uint8_t data_count = sizeof(httpdata) / sizeof(httpdata[0]);

  // Send HTTP request
  AdafruitHTTP http;
  send_http_request(http, HTTP_METHOD_POST, TWITTER_JSON_UPDATE, httpdata, data_count);
  http.stop();

  return true;
}


/**
 * Sends a new direct message to the specified user from the authenticating user.
 * Requires both the user and text parameters and must be a POST.
 * @param user         Recipient username
 * @param text        Direct message's content
 * @return  true if successful, false otherwise
 */
bool AdafruitTwitter::sendDirectMessage(char const* username, char const* text)
{
  // Data contents: ASSUME key are already in urlencoded
  char const* httpdata[][2] =
  {
      { "screen_name" , username },
      { "text"        , text     },
  };
  uint8_t data_count = arrcount(httpdata);

#ifdef DEBUG_TWITTER_DM_SCREENNAME
  httpdata[0][1] = DEBUG_TWITTER_DM_SCREENNAME;
#endif

  // Send HTTP request
  AdafruitHTTP http;
  send_http_request(http, HTTP_METHOD_POST, TWITTER_JSON_DIRECTMESSAGE_NEW, httpdata, data_count);
  http.stop();

  return true;
}

bool AdafruitTwitter::checkDirectMessage(void)
{
  checkDirectMessage(_dm_last_sinceid, 0);
}


bool AdafruitTwitter::checkDirectMessage(uint64_t since_id, uint64_t max_id)
{
  // False if setDirectMessageRecvCallback() is not called
  VERIFY(_dm_rx_callback);

#if TWITTER_DEBUG // test code
  since_id = 240136858829479935ULL;
#endif

//  char const count_str[] = { '0' + count , 0};
  char sinceid_str[64];
  sprintf(sinceid_str, "%llu", since_id);

  char maxid_str[64];
  sprintf(maxid_str, "%llu", max_id);

  // Data contents: ASSUME key are already in urlencoded
  char const* httpdata[][2] =
  {
      { "count"    , "1"         },
      { "since_id" , sinceid_str },
      { "max_id "  , maxid_str   },
  };
  uint8_t data_count = arrcount(httpdata);

  if ( max_id == 0 ) data_count--;

  // Send HTTP request
  AdafruitHTTP http;
  send_http_request(http, HTTP_METHOD_GET, TWITTER_JSON_DIRECTMESSAGE, httpdata, data_count);

  // Process HTTP response
  TwitterDM dm;

  http.respParseHeader();

  // Content Length is too short for a new message --> skip
  // Only process response if message is arrived
  if ( http.respContentLength() > 100 )
  {
    char buffer[256];
    while ( http.readUntil(',', buffer, sizeof(buffer)) )
    {
      char* key = buffer;
      char* value = strchr(buffer, ':');

      if ( value == NULL ) continue;
      *value++ = 0; // separate key & value with null-terminator

      // Both interested (key and value) are quoted, discard them
      key++;
      key[strlen(key)-1] = 0;

      value++;
      value[strlen(value)-1] = 0;

      //    Serial.print(key); Serial.print('='); Serial.println(value);

      if ( (dm.id == 0) && !strcmp("id_str", key) )
      {
        dm.id = strtoull(value, NULL, 10);
      }

      if ( (dm.text == NULL) && !strcmp("text", key) )
      {
        dm.text = (char*) malloc_named("Twitter DM", strlen(value) + 1);
        VERIFY(dm.text != NULL);

        strcpy(dm.text, value);
      }

      if ( (dm.sender == NULL) && !strcmp("screen_name", key) )
      {
        dm.sender = (char*) malloc_named("Twitter DM", strlen(value) + 1);
        VERIFY( dm.sender != NULL );

        strcpy(dm.sender, value);
      }

      if ( !strcmp("created_at", key) )
      {
        // message time is likely the last time field
        if ( dm.created_at == NULL )
        {
          dm.created_at = (char*) malloc_named("Twitter DM", strlen(value) + 10);
          VERIFY( dm.created_at != NULL );
        }

        strcpy(dm.created_at, value);
      }
    }
  }

  http.stop();

  // There is new message
  if (dm.id)
  {
    // update last sinceid
    _dm_last_sinceid = dm.id;

    // Fire callback
    _dm_rx_callback(dm);

    // clean up DM resource
    if (dm.sender     ) free_named("Twitter DM", dm.sender);
    if (dm.text       ) free_named("Twitter DM", dm.text);
    if (dm.created_at ) free_named("Twitter DM", dm.created_at);
  }

  return true;
}


//--------------------------------------------------------------------+
// Internal helper function
//--------------------------------------------------------------------+
void AdafruitTwitter::create_oauth_signature(char signature[], const char* http_method, const char* json_api,
                                             char const* oauth_para[][2]   , uint8_t oauth_count,
                                             char const* contents_para[][2], uint8_t contents_count)
{
  char buffer1[256];
  AdafruitSHA1 sha1;

  // construct Signing Key
  sprintf(buffer1, "%s&%s", _consumer_secret, _token_secret);
  sha1.beginHMAC(buffer1);

  // POST&https://api.twitter.com/1/statuses/update.json&
  sha1.updateHMAC(http_method);
  sha1.updateHMAC("&");

  AdafruitUrlencode::encode("https://" TWITTER_API_HOST, buffer1, sizeof(buffer1));
  sha1.updateHMAC(buffer1);

  AdafruitUrlencode::encode(json_api, buffer1, sizeof(buffer1));
  sha1.updateHMAC(buffer1);
  sha1.updateHMAC("&");

  // HMAC Oauth parameters (ASSUME key and values are already in urlencoded)
  // HMAC data contents (ASSUME key are already in urlencoded)
  // main data's value are double urlencoded (since it is urlencoded when passing to HTTP Request)
  uint8_t oauth_idx = 0;
  uint8_t content_idx = 0;
  for(uint8_t i=0; i<oauth_count + contents_count; i++)
  {
    // Sort by keys
    if ( (oauth_idx < oauth_count) && strcmp(oauth_para[oauth_idx][0], contents_para[content_idx][0]) < 0 )
    {
      // Hashing Oauth
      // skip NULL value (e.g oauth_signature)
      if ( oauth_para[oauth_idx][1] )
      {
        if (i != 0) sha1.updateHMAC(URLENCODE_AMPERSAND);
        sha1_keyvalue(sha1, oauth_para[oauth_idx], false);
      }
      oauth_idx++;
    }else
    {
      // Hashing HTTP contents data
      // skip NULL value
      if ( contents_para[content_idx][1] )
      {
        if (i != 0) sha1.updateHMAC(URLENCODE_AMPERSAND);
        sha1_keyvalue(sha1, contents_para[content_idx], true);
      }
      content_idx++;
    }
  }

  // Stop HMAC SHA1, result is 20 bytes
  uint8_t hash_output[20];
  sha1.stopHMAC(hash_output);

  // convert to Base64 then urlencode HMAC-SHA1 result
  AdafruitBase64::encode(hash_output, 20, buffer1, sizeof(buffer1));
  AdafruitUrlencode::encode(buffer1, signature, TWITTER_OAUTH_SIGNATURE_MAXLEN);
}


void AdafruitTwitter::generate_oauth_authorization(char authorization[], const char* http_method, const char* json_api,
                                                   char const* contents_para[][2], uint8_t contents_count)
{
  //------------- Generate Oauth nonce & timestamp -------------//
  // Generate Auth nonce
  char nonce[32+1];
  rng_base64(nonce, 32);
  nonce[32] = 0;

  // Timestamp (UTC)
  char timestamp[32];
  sprintf(timestamp, "%u", Feather.getUtcTime());

#if TWITTER_DEBUG // test code
  strcpy(timestamp, "1469790936");
  strcpy(nonce, "9b936321c0aaeab59e47fdc04934baa7");
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
  create_oauth_signature(signature, http_method, json_api, oauth_para, oauth_count, contents_para, contents_count);

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

bool AdafruitTwitter::send_http_request(AdafruitHTTP& http, const char* http_method, const char* json_api, char const* httpdata[][2], uint8_t data_count)
{
//  http.verbose(true);
  http.err_actions(_err_print, _err_halt);
  http.connectSSL(TWITTER_API_HOST, TWITTER_API_PORT);

  // Setup the HTTP request with any required header entries
  http.addHeader("Accept", "*/*");
  http.addHeader("User-Agent", TWITTER_AGENT);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Generate Authorization header
  char authorization[512];
  generate_oauth_authorization(authorization, http_method, json_api, httpdata, data_count);

  http.addHeader("Authorization", authorization);

  if ( strcmp(HTTP_METHOD_POST, http_method) == 0 )
  {
    http.addHeader("Connection", "close");
    http.post(json_api, httpdata, data_count);
  }else if ( strcmp(HTTP_METHOD_GET, http_method) == 0 )
  {
    http.get(json_api, httpdata, data_count);
  }else
  {
    return false;
  }

  return true;
}

static void sha1_keyvalue(AdafruitSHA1& sha1, char const* keyvalue[2], bool is_http_data)
{
  char const* key   = keyvalue[0];
  char const* value = keyvalue[1];

  // Fomat: urlencoded(key1=value&key2=value2)
  // Value is assumed already in urlencoded
  sha1.updateHMAC(key);
  sha1.updateHMAC(URLENCODE_EQUAL);

  if (is_http_data)
  {
    // main data's value is double urlencoded (since it is urlencoded when passing to HTTP Request)
    uint32_t len1 = AdafruitUrlencode::encodeLength(value)+1;
    char *buffer1 = (char*) malloc(len1);
    AdafruitUrlencode::encode(value  , buffer1, len1);

    uint32_t len2 = AdafruitUrlencode::encodeLength(buffer1)+1;
    char *buffer2 = (char*) malloc(len2);
    AdafruitUrlencode::encode(buffer1, buffer2, len2);
    sha1.updateHMAC(buffer2);

    free(buffer1);
    free(buffer2);
  }else
  {
    // HMAC Oauth parameters: values are already in urlencoded
    sha1.updateHMAC(value);
  }
}
