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
#include <Client.h>
#include <IPAddress.h>
#include <adafruit_feather.h>

class MQTTString
{
public:
  int len;
	const char* data;

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
};

class AdafruitMQTT : public AdafruitTCP
{
protected:
  const char * _clientID;
  const char * _username;
  const char * _password;

  MQTTString _will_topic;
  MQTTString _will_message;
  uint8_t    _will_retained;
  uint8_t    _will_qos;

public:
	AdafruitMQTT(const char * clientID, const char* username, const char* password)
	{
	  _packet_buffering = true;
	  _clientID = clientID;
	  _username = username;
	  _password = password;
	}

	AdafruitMQTT(const char* username, const char* password)
	{
	  _packet_buffering = true;
	  _username = username;
	  _password = password;
	  _clientID = NULL;
	}

	AdafruitMQTT(const char * clientID)
	{
	  _packet_buffering = true;
	  _clientID = clientID;
	  _username = _password = NULL;
	}

	AdafruitMQTT()
	{
	  _packet_buffering = true;
	  _clientID = _username = _password = NULL;
	}

  // Set MQTT last will topic, payload, QOS, and retain. This needs
  // to be called before connect() because it is sent as part of the
  // connect control packet.
	void will(MQTTString topic, MQTTString message, uint8_t qos =0, uint8_t retained =0)
	{
	  _will_topic    = topic;
	  _will_message  = message;
	  _will_qos      = qos;
	  _will_retained = retained;
	}

	// Overwrite AdafruitTCP connect()
  virtual int connect   (IPAddress ip, uint16_t port);
  virtual int connectSSL(IPAddress ip, uint16_t port);

};



#endif /* _ADAFRUIT_MQTT_H__ */
