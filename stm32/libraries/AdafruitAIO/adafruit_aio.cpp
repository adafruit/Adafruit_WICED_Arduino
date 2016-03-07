/**************************************************************************/
/*!
    @file     adafrtuit_aio.cpp
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

#include <adafruit_feather.h>
#include <adafruit_mqtt.h>
#include "adafruit_aio.h"

// Debugging purpose
#if defined(DEBUG_AIO_USERNAME) && defined(DEBUG_AIO_KEY)
AdafruitAIO::AdafruitAIO(const char* username, const char* password) : AdafruitMQTT(DEBUG_AIO_USERNAME, DEBUG_AIO_KEY)
#else
AdafruitAIO::AdafruitAIO(const char* username, const char* password) : AdafruitMQTT(username, password)
#endif
{

}

bool AdafruitAIO::connect(bool cleanSession, uint16_t keepalive_sec)
{
  VERIFY(_username != NULL && _password != NULL);
  return this->connect(AIO_SERVER, AIO_UNSECURED_PORT, cleanSession, keepalive_sec);
}

bool AdafruitAIO::connectSSL(bool cleanSession, uint16_t keepalive_sec)
{
  VERIFY(_username != NULL && _password != NULL);
  return this->connectSSL(AIO_SERVER, AIO_SECURED_PORT, cleanSession, keepalive_sec);
}

char* AdafruitAIO::createFeed(const char* feedid)
{
  // feed path = username/feeds/feedid
  char * feedfull = (char*) malloc( strlen(_username) + 7 + strlen(feedid) + 1 );
  strcpy(feedfull, _username);
  strcat(feedfull, "/feeds/");
  strcat(feedfull, feedid);

  return feedfull;
}

void  AdafruitAIO::removeFeed(char* feedfull)
{
  free(feedfull);
}

bool AdafruitAIO::updateFeed(const char* feed, UTF8String message, uint8_t qos, bool retain)
{
  char * feedfull = createFeed(feed);
  bool result = this->publish(feedfull, message, qos, retain);
  removeFeed(feedfull);

  return result;
}

bool AdafruitAIO::followFeed(const char* feed, uint8_t qos, feedHandler_t mh)
{
  char * feedfull = createFeed(feed);
  bool result = this->subscribe_internal(feedfull, qos, (void*) mh, true, NULL);
  removeFeed(feedfull);

  return result;
}

bool AdafruitAIO::followFeed(const char* feed, uint8_t qos, feedHandler_t mh, AdafruitAIOFeed* aio_feed)
{
  char * feedfull = createFeed(feed);
  bool result = this->subscribe_internal(feedfull, qos, (void*) mh, true, (void*) aio_feed);
  removeFeed(feedfull);

  return result;
}

bool AdafruitAIO::unfollowFeed(const char* feed)
{
  char * feedfull = createFeed(feed);
  bool result = this->unsubscribe(feedfull);
  removeFeed(feedfull);

  return result;
}

