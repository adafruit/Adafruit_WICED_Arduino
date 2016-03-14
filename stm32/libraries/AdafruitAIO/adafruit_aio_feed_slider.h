/**************************************************************************/
/*!
    @file     adafruit_aio_feed_slider.h
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

#ifndef _ADAFRUIT_AIO_FEED_GAUGE_H_
#define _ADAFRUIT_AIO_FEED_GAUGE_H_

// Feed Guage is exactly
#define AdafruitAIOFeedGauge AdafruitAIOFeedSlider

template <typename Num>
class AdafruitAIOFeedSlider : public AdafruitAIOFeed
{
protected:
  Num _value;
  uint8_t _precision;

  virtual void subscribed_callback(UTF8String topic, UTF8String message, void* callback_func);

  // Specialization function
  void mess2num(char* str, float* p_value)
  {
    (*p_value) = strtof(str, NULL);
  }

  void mess2num(char* str, int* p_value)
  {
    (*p_value) = strtol(str, NULL, 0);
  }

  void updateValue (int value  ) { print(value); }
  void updateValue (float value) { print((double) value, _precision); }

public:
  typedef void (*feedNumberHandler_t)(Num value);

  AdafruitAIOFeedSlider(AdafruitAIO* aio, const char* feed, uint8_t qos = MQTT_QOS_AT_MOST_ONCE, bool retain = true)
    : AdafruitAIOFeed(aio, feed, qos, retain)
  {
    _precision = 2;
  }

  void floatPrecision(uint8_t p) { _precision = p; }

  bool follow  (feedNumberHandler_t fp) { this->follow((feedHandler_t) fp); }
  bool follow  (void) { return this->follow((feedHandler_t)NULL); }

  Num  operator =  (Num value);
  bool operator == (Num value) { this->_value == value; }
  bool operator != (Num value) { return ! ((*this) == value); }

  operator Num()  { return this->_value; }
  Num value(void) { return this->_value; }

  // Inherit from AIOFeed
  bool unfollow(void);
  bool followed(void);

  using AdafruitAIOFeed::follow;
  using AdafruitAIOFeed::write;
};

#endif /* _ADAFRUIT_AIO_FEED_GAUGE_H_ */
