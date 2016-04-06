/**************************************************************************/
/*!
    @file     adafruit_aio_feed.cpp
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
#include "adafruit_aio_feed.h"

/******************************************************************************/
/*!
    @brief Constructor
*/
/******************************************************************************/
AdafruitAIOFeed::AdafruitAIOFeed(AdafruitAIO* aio, const char* feed, uint8_t qos, bool retain)
: AdafruitMQTTTopic(aio, feed, qos, retain)
{
  _aio = aio;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitAIOFeed::follow(feedHandler_t mh)
{
  return _aio->followFeed(_topic, _qos, mh, this);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitAIOFeed::unfollow(void)
{
  return _aio->unfollowFeed(_topic);
}

/******************************************************************************/
/*!
    @brief Callback fired when message arrived if this pointer is passed to featherlib
*/
/******************************************************************************/
void AdafruitAIOFeed::subscribed_callback(UTF8String topic, UTF8String message, void* callback_func)
{
  if ( callback_func )
  {
    feedHandler_t mh = (feedHandler_t) callback_func;
    mh(message);
  }
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
size_t AdafruitAIOFeed::write(const uint8_t *buf, size_t len)
{
  bool result = _aio->updateFeed(_topic, UTF8String(buf, len), _qos, _retained);
  return result ? len : 0;
}

