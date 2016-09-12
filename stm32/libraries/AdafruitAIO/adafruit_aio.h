/**************************************************************************/
/*!
    @file     adafruit_aio.h
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

#ifndef _ADAFRUIT_AIO_H_
#define _ADAFRUIT_AIO_H_

#include <Arduino.h>

#define AIO_SERVER          "io.adafruit.com"
#define AIO_UNSECURED_PORT  1883
#define AIO_SECURED_PORT    8883

// forward declaration
class AdafruitAIOFeed;

class AdafruitAIO : public AdafruitMQTT
{
protected:
  char* createFeed(const char* feedid);
  void  removeFeed(char* feedfull);

public:
  typedef void (*feedHandler_t)(UTF8String message);
  AdafruitAIO(const char* username, const char* password);

  bool connect   (bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);
  bool connectSSL(bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);
  using AdafruitMQTT::connect;
  using AdafruitMQTT::connectSSL;

  bool updateFeed  (const char* feed, UTF8String message, uint8_t qos=MQTT_QOS_AT_MOST_ONCE, bool retain=true);
  bool followFeed  (const char* feed, uint8_t qos, feedHandler_t mh);
  bool unfollowFeed(const char* feed);

  // for internal use
  bool followFeed(const char* feed, uint8_t qos, feedHandler_t mh, AdafruitAIOFeed* aio_feed);
};

#include "adafruit_aio_feed.h"
#include "adafruit_aio_feed_onoff.h"
#include "adafruit_aio_feed_pushbutton.h"
#include "adafruit_aio_feed_slider.h"
#include "adafruit_aio_feed_text.h"
#include "adafruit_aio_feed_color.h"

#endif /* _ADAFRUIT_AIO_H_ */
