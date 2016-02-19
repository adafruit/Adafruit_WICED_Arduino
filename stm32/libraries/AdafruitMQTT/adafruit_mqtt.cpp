/**************************************************************************/
/*!
    @file     adafruit_mqtt.cpp
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

#include "adafruit_mqtt.h"

bool AdafruitMQTT::connectBroker(void)
{
  uint32_t tcp_handle = tcp.getHandle();

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4                   , .p_value = &tcp_handle        },
      { .len = 2                   , .p_value = &_keepalive_sec    },
      { .len = 1                   , .p_value = &_cleansession     },
      // ID                        , user, pass
      { .len = strlen(_clientID)   , .p_value = _clientID          },
      { .len = strlen(_username)   , .p_value = _username          },
      { .len = strlen(_password)   , .p_value = _password          },
      // will para
      { .len = strlen(_will_topic) , .p_value = _will_topic        },
      { .len = _will_message.len   , .p_value = _will_message.data },
      { .len = 1                   , .p_value = &_will_qos         },
      { .len = 1                   , .p_value = &_will_retained    },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_MQTTCONNECT, para_count, para_arr, NULL, &_mqtt_handle);
}

bool AdafruitMQTT::connect ( IPAddress ip, uint16_t port )
{
  VERIFY ( tcp.connect(ip, port) );
  _connected = connectBroker();

  // close TCP if failed to connect
  if (!_connected) tcp.stop();

  return _connected;
}

bool AdafruitMQTT::connect ( const char * host, uint16_t port )
{
  IPAddress ip;
  VERIFY( Feather.hostByName(host, ip) );
  return this->connect(ip, port);
}


bool AdafruitMQTT::connectSSL(IPAddress ip, uint16_t port)
{
  // Call AdafruitTCP connect
  VERIFY( tcp.connectSSL(ip, port) );
  _connected = connectBroker();

  // close TCP if failed to connect
  if (!_connected) tcp.stop();

  return _connected;
}

bool AdafruitMQTT::connectSSL ( const char* host, uint16_t port )
{
  IPAddress ip;
  VERIFY( Feather.hostByName(host, ip) );
  return this->connectSSL(ip, port);
}

bool AdafruitMQTT::disconnect ( void )
{
  // skip if not connected
  if ( !_connected ) return true;

  // Send Disconnect Packet to Broker
  VERIFY( sdep(SDEP_CMD_MQTTDISCONNECT, 4, &_mqtt_handle, NULL, NULL) );
  _mqtt_handle = 0;

  // Disconnect TCP
  tcp.stop();

  _connected = false;
  return true;
}

bool AdafruitMQTT::publish( const char* topic, MQTTString message, uint8_t qos, bool retained )
{
  VERIFY( _connected );

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4             , .p_value = &_mqtt_handle },
      { .len = 1             , .p_value = &qos          },
      { .len = 1             , .p_value = &retained     },
      { .len = strlen(topic) , .p_value = topic         },
      { .len = message.len   , .p_value = message.data  },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_MQTTPUBLISH, para_count, para_arr, NULL, NULL);
}

