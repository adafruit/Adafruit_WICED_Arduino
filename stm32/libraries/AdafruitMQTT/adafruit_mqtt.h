/**************************************************************************/
/*!
    @file     adafruit_mqtt.h
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

#ifndef _ADAFRUIT_MQTT_H__
#define _ADAFRUIT_MQTT_H__

#include <Arduino.h>
#include <Printable.h>
#include <Client.h>
#include <IPAddress.h>
#include <adafruit_feather.h>

enum
{
  MQTT_QOS_AT_MOST_ONCE,
  MQTT_QOS_AT_LEAST_ONCE,
  MQTT_QOS_EXACTLY_ONCE,
};

#define MQTT_KEEPALIVE_DEFAULT    60 // in seconds

class MQTTString : public Printable
{
public:
  int len;
  const char* data;

  MQTTString(void) { len = 0; data = NULL; }

  MQTTString(const char* cstring            ) { len = strlen(cstring); data = cstring;  }
  MQTTString(const char* bytes, int size    ) { len = size; data = bytes;               }
  MQTTString(const uint8_t* bytes, int size ) { len = size; data = (const char*) bytes; }

  MQTTString& operator = (const MQTTString& rhs)
  {
    if (this == &rhs) return *this;

    this->len = rhs.len;
    this->data = rhs.data;

    return *this;
  }

  bool operator == (const char *cstr) const { return (strlen(cstr) == len) && !memcmp(data, cstr, len); }

  virtual size_t printTo(Print& p) const
  {
    p.write(this->data, len);
  }

  explicit operator const char*() const { return data; }
  explicit operator const uint8_t*() const { return ( const uint8_t*) data; }
  explicit operator const unsigned char*() const { return (const unsigned char*) data; }
};

class AdafruitMQTT : public AdafruitSDEP
{
protected:
  AdafruitTCP tcp;
  uint32_t  _mqtt_handle;

  bool _connected;

  // Connect Packet Data
  const char * _clientID;
  const char * _username;
  const char * _password;

  const char* _will_topic;
  MQTTString  _will_message;
  uint8_t     _will_qos;
  uint8_t     _will_retained;


  bool connectBroker(bool cleanSession, uint16_t keepalive_sec);
  bool init(void)
  {
    _mqtt_handle   = 0;
    _connected     = 0;
    tcp.usePacketBuffering(true);

    _clientID = _username = _password = NULL;

    _will_qos = _will_retained = 0;
  }

public:
  // messageHandler must not be changed since featherlib uses the same signature
  typedef void (*messageHandler)(char* topic_data, size_t topic_len, uint8_t* mess_data, size_t mess_len);

  AdafruitMQTT(const char * clientID, const char* username, const char* password)
  {
    init();
    _clientID = clientID;
    _username = username;
    _password = password;
  }

  AdafruitMQTT(const char* username, const char* password)
  {
    init();
    _username = username;
    _password = password;
  }

  AdafruitMQTT(const char * clientID)
  {
    init();
    _clientID = clientID;
  }

  AdafruitMQTT()
  {
    init();
  }

  // Set MQTT last will topic, payload, QOS, and retain. This needs
  // to be called before connect() because it is sent as part of the
  // connect control packet.
  void will(const char* topic, MQTTString message, uint8_t qos =MQTT_QOS_AT_MOST_ONCE, uint8_t retained =0)
  {
    _will_topic    = topic;
    _will_message  = message;
    _will_qos      = qos;
    _will_retained = retained;
  }

  // API
  bool connected  ( void ) { return _connected; }

  bool connect    ( IPAddress ip, uint16_t port = 1883, bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);
  bool connect    ( const char * host, uint16_t port = 1883,  bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);
  bool connectSSL ( IPAddress ip, uint16_t port = 8883, bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);
  bool connectSSL ( const char* host, uint16_t port = 8883, bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);

  bool disconnect ( void );

  bool publish    ( const char* topic, MQTTString message, uint8_t qos =MQTT_QOS_AT_MOST_ONCE, bool retained = false );
  bool subscribe  ( const char* topicFilter, uint8_t qos, messageHandler mh);
  bool unsubscribe( const char* topicFilter );
};

#endif /* _ADAFRUIT_MQTT_H__ */
