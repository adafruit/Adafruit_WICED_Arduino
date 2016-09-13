/**************************************************************************/
/*!
    @file     adafruit_aio_feed_text.cpp
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
AdafruitAIOFeedText::AdafruitAIOFeedText(AdafruitAIO* aio, const char* feed, uint8_t qos, bool retain):
  AdafruitAIOFeed(aio, feed, qos, retain)
{
  _str = NULL;
}

/******************************************************************************/
/*!
    @brief Destructor
*/
/******************************************************************************/
AdafruitAIOFeedText::~AdafruitAIOFeedText()
{
  if (_str) free (_str);
}

/******************************************************************************/
/*!
    @brief Helper to update internal value
*/
/******************************************************************************/
bool AdafruitAIOFeedText::updateValue(UTF8String message)
{
  // If value is different than the current non-null one
  if ( _str && (message != _str) )
  {
    free(_str); // free old value if any
    _str = NULL;
  }

  // malloc to hold a copy of value, skip if equal
  if ( _str == NULL )
  {
    _str = (char*) malloc( message.len + 1);
    VERIFY(_str != NULL);

    // copy contents and add null terminator
    memcpy(_str, message.data, message.len);
    _str[message.len] = 0;
  }

  return true;
}

/******************************************************************************/
/*!
    @brief Follow with callback
*/
/******************************************************************************/
bool AdafruitAIOFeedText::follow(feedTextHandler_t fp)
{
  return this->follow((feedHandler_t) fp);
}

/******************************************************************************/
/*!
    @brief Assignment overload
*/
/******************************************************************************/
bool AdafruitAIOFeedText::operator = (char const* value)
{
  this->updateValue(value);
  return this->write(value) > 0;
}

/******************************************************************************/
/*!
    @brief Equality overload
*/
/******************************************************************************/
bool AdafruitAIOFeedText::operator == (char const* value)
{
  return (strcmp(_str, value) == 0);
}

/******************************************************************************/
/*!
    @brief Subcribed callback
*/
/******************************************************************************/
void AdafruitAIOFeedText::subscribed_callback(UTF8String topic, UTF8String message, void* callback_func)
{
  (void) topic;

  this->updateValue(message);

  if ( callback_func )
  {
    feedTextHandler_t mh = (feedTextHandler_t) callback_func;
    mh(_str);
  }
}
