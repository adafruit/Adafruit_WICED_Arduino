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
#include <adafruit_sdep.h>

#define MQTT_KEEPALIVE_DEFAULT    60 // in seconds
enum
{
  MQTT_QOS_AT_MOST_ONCE,
  MQTT_QOS_AT_LEAST_ONCE,
  MQTT_QOS_EXACTLY_ONCE,
};

#include "adafruit_mqtt_topic.h"

// Callback proxy from Featherlib
extern "C"
{
  void adafruit_mqtt_disconnect_callback(void* p_mqtt);
}

class AdafruitMQTT : public AdafruitSDEP
{
public:
  typedef void (*messageHandler_t)(UTF8String topic, UTF8String message);

  AdafruitMQTT(const char* username, const char* password)
  {
    init();
    _username = username;
    _password = password;
  }

  AdafruitMQTT()
  {
    init();
  }

  // Set MQTT last will topic, payload, QOS, and retain. This needs
  // to be called before connect() because it is sent as part of the
  // connect control packet.
  void will(const char* topic, UTF8String message, uint8_t qos =MQTT_QOS_AT_MOST_ONCE, uint8_t retained =0)
  {
    _will_topic    = topic;
    _will_message  = message;
    _will_qos      = qos;
    _will_retained = retained;
  }

  void clientID(const char* client) { _clientID = client; }
  void setDisconnectCallback  ( void (*fp) (void) ) { _disconnect_callback = fp; }
  bool tlsSetIdentity         (char const* private_key, uint8_t const* local_cert, uint16_t local_certlen)
  {
    return tcp.tlsSetIdentity(private_key, local_cert, local_certlen);
  }

  // API
  bool connected      ( void ) { return _connected; }

  bool connect        ( IPAddress ip     , uint16_t port = 1883, bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);
  bool connect        ( const char* host , uint16_t port = 1883, bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);
  bool connectSSL     ( IPAddress ip     , uint16_t port = 8883, bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);
  bool connectSSL     ( const char* host , uint16_t port = 8883, bool cleanSession = true, uint16_t keepalive_sec = MQTT_KEEPALIVE_DEFAULT);

  bool disconnect     ( void );

  bool publish        ( UTF8String topic, UTF8String message, uint8_t qos =MQTT_QOS_AT_MOST_ONCE, bool retained = false );
  bool subscribe      ( const char* topicFilter, uint8_t qos, messageHandler_t mh);
  bool subscribe_copy ( const char* topicFilter, uint8_t qos, messageHandler_t mh);
  bool unsubscribe    ( const char* topicFilter );

  // SDEP
  virtual void err_actions (bool print, bool halt)
  {
    tcp.err_actions(print, halt);
    _err_print = print;
    _err_halt  = halt;
  }

  friend void adafruit_mqtt_disconnect_callback(void* p_mqtt);

protected:
  AdafruitTCP tcp;
  uint32_t  _mqtt_handle;

  bool _connected;

  // Connect Packet Data
  const char * _clientID;
  const char * _username;
  const char * _password;

  const char* _will_topic;
  UTF8String  _will_message;
  uint8_t     _will_qos;
  uint8_t     _will_retained;

  void (*_disconnect_callback) (void);

  bool connectBroker(bool cleanSession, uint16_t keepalive_sec);
  bool init(void)
  {
    _mqtt_handle   = 0;
    _connected     = 0;
    tcp.usePacketBuffering(true);

    _disconnect_callback = NULL;
    _clientID = _username = _password = NULL;
    _will_qos = _will_retained = 0;
  }

  void randomClientID(char* clientid);

  bool subscribe_internal(const char* topicFilter, uint8_t qos, void* mh, bool copy_topic, void* arg=NULL);
  void disconnect_cleanup(void);
};

#endif /* _ADAFRUIT_MQTT_H__ */
