/**************************************************************************/
/*!
    @file     adafruit_mqtt_topic.cpp
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

#include <Arduino.h>
#include <adafruit_feather.h>
#include "adafruit_mqtt.h"
#include "adafruit_mqtt_topic.h"

/******************************************************************************/
/*!
    @brief Constructor
*/
/******************************************************************************/
AdafruitMQTTTopic::AdafruitMQTTTopic(AdafruitMQTT* mqtt, const char* topic, uint8_t qos, bool retain)
{
  _mqtt       = mqtt;
  _topic      = topic;
  _qos        = qos;
  _retained   = retain;

  _subscribed = false;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
size_t AdafruitMQTTTopic::write(const uint8_t *buf, size_t len)
{
  if ( !_subscribed ) return 0;
  return _mqtt->publish(_topic, UTF8String(buf, len), _qos, _retained) ? len : 0;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTTTopic::subscribe(messageHandler_t mh)
{
  _subscribed = _mqtt->subscribe(_topic, _qos, mh);
  return _subscribed;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTTTopic::unsubscribe(void)
{
  _subscribed = !_mqtt->unsubscribe(_topic);
  return !_subscribed;
}

/******************************************************************************/
/*!
    @brief Callback fired when messag arrived if this pointer is passed to featherlib
*/
/******************************************************************************/
void AdafruitMQTTTopic::subscribed_callback(UTF8String topic, UTF8String message, void* callback_func)
{
  // default implementation is just called the callback function with topic and message
  if ( callback_func )
  {
    messageHandler_t mh = (messageHandler_t) callback_func;
    mh(topic, message);
  }
}

//--------------------------------------------------------------------+
// Callback
//--------------------------------------------------------------------+
void  adafruit_mqtt_subscribed_callback(char* topic_data, size_t topic_len, uint8_t* mess_data, size_t len, void* callback_func, void* arg)
{
  // no AdafruitMQTTTopic's this pointer, use default message handler signature in MQTT
  UTF8String topic(topic_data, topic_len);
  UTF8String message(mess_data, len);

  if ( arg == NULL )
  {
    if ( callback_func )
    {
      AdafruitMQTT::messageHandler_t mh = (AdafruitMQTT::messageHandler_t) callback_func;
      mh(topic, message);
    }
  }else
  {
    AdafruitMQTTTopic* p_topic = (AdafruitMQTTTopic*) arg;
    p_topic->subscribed_callback(topic, message, callback_func);
  }
}

