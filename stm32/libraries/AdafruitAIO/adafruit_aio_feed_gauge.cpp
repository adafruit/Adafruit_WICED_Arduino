/**************************************************************************/
/*!
    @file     adafruit_aio_feed_gauge.cpp
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
#include <typeinfo>

/******************************************************************************/
/*!
    @brief Constructor
*/
/******************************************************************************/
template <class Num>
AdafruitAIOFeedGauge<Num>::AdafruitAIOFeedGauge(AdafruitAIO* aio, const char* feed, uint8_t qos, bool retain)
  : AdafruitAIOFeed(aio, feed, qos, retain)
{

}

/******************************************************************************/
/*!
    @brief Assignment overload
*/
/******************************************************************************/
template <class Num>
Num AdafruitAIOFeedGauge<Num>::operator = (Num value)
{
  _value = value;
//  this->write( value ? "ON" : "OFF");
}

/******************************************************************************/
/*!
    @brief Equation overload
*/
/******************************************************************************/
template <class Num>
bool AdafruitAIOFeedGauge<Num>::operator == (Num value)
{
  return this->_value == value;
}

/******************************************************************************/
/*!
    @brief Follow with callback
*/
/******************************************************************************/
template <class Num>
bool AdafruitAIOFeedGauge<Num>::follow(feedNumberHandler_t fp)
{
  return this->follow((feedHandler_t) fp);
}

/******************************************************************************/
/*!
    @brief Follow without callback
*/
/******************************************************************************/
template <class Num>
bool AdafruitAIOFeedGauge<Num>::follow(void)
{
  return this->follow((feedHandler_t)NULL);
}

/******************************************************************************/
/*!
    @brief Callback fired when message arrived if this pointer is passed to featherlib
*/
/******************************************************************************/
template <class Num>
void AdafruitAIOFeedGauge<Num>::subscribed_callback(UTF8String topic, UTF8String message, void* callback_func)
{
  (void) topic;

  // malloc to have an null-terminated byte
  char * str = (char*) malloc (message.len + 1);
  memcpy(str, message.data, message.len);
  str[message.len] = 0;

  if ( typeid(float) == typeid(Num) )
  {
    _value = strtof(str, NULL);
  }else if ( typeid(Num) == typeid(int32_t) ||
             typeid(Num) == typeid(uint32_t) ||
             typeid(Num) == typeid(int) ||
             typeid(Num) == typeid(long) )
  {
    _value = strtol(str, NULL, 0);
  }else
  {
    // do nothing
  }

  if ( callback_func )
  {
    feedNumberHandler_t mh = (feedNumberHandler_t) callback_func;
    mh(_value);
  }
}
