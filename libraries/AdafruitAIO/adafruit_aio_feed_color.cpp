/**************************************************************************/
/*!
    @file     adafruit_aio_feed_color.cpp
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

/******************************************************************************/
/*!
    @brief Constructor
*/
/******************************************************************************/
AdafruitAIOFeedColor::AdafruitAIOFeedColor(AdafruitAIO* aio, const char* feed, uint8_t qos, bool retain):
  AdafruitAIOFeed(aio, feed, qos, retain)
{
  memclr(_rgb, 3);
}

/******************************************************************************/
/*!
    @brief Assignment overload
*/
/******************************************************************************/
bool AdafruitAIOFeedColor::update(const uint8_t rgb[3])
{
  memcpy(_rgb, rgb, 3);

  char message[8];
  sprintf(message, "#%02X%02X%02X", _rgb[0], _rgb[1], _rgb[2]);

  return this->write(message) > 0;
}

/******************************************************************************/
/*!
    @brief Follow with callback
*/
/******************************************************************************/
bool AdafruitAIOFeedColor::follow(feedColorHandler_t fp)
{
  return this->follow((feedHandler_t) fp);
}

/******************************************************************************/
/*!
    @brief Subcribed callback
*/
/******************************************************************************/
void AdafruitAIOFeedColor::subscribed_callback(UTF8String topic, UTF8String message, void* callback_func)
{
  (void) topic;

  // First byte is #
  if(message.data[0] != '#') return;

  char c[3] = { 0 };
  for(uint8_t i=0; i<3; i++)
  {
    memcpy(c, &message.data[i*2+1], 2); // 1, 3, 5
    _rgb[i] = (uint8_t) strtol(c, NULL, 16);
  }

  if ( callback_func )
  {
    feedColorHandler_t mh = (feedColorHandler_t) callback_func;
    mh(_rgb);
  }
}
