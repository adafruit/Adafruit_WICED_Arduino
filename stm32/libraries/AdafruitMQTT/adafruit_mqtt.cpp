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

#include <adafruit_feather.h>
#include "adafruit_mqtt.h"
#include "rng.h"

void AdafruitMQTT::randomClientID(char* clientid)
{
  // length is from 10 to 23
  uint8_t length = 10+(rng_u32()%13);
  rng_base64(clientid, length);
  clientid[length] = 0;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::connectBroker(bool cleanSession, uint16_t keepalive_sec)
{
  uint32_t tcp_handle = tcp.getHandle();

  // _clientID is not set, generate an random for it
  char random_id[24];
  if (_clientID == NULL)
  {
    this->randomClientID(random_id);
    _clientID = random_id;
  }

  uint32_t this_value = (uint32_t) this;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4                   , .p_value = &tcp_handle        },
      { .len = 2                   , .p_value = &keepalive_sec     },
      { .len = 1                   , .p_value = &cleanSession      },
      // ID                        , user, pass
      { .len = strlen(_clientID)   , .p_value = _clientID          },
      { .len = strlen(_username)   , .p_value = _username          },
      { .len = strlen(_password)   , .p_value = _password          },
      // will para
      { .len = strlen(_will_topic) , .p_value = _will_topic        },
      { .len = _will_message.len   , .p_value = _will_message.data },
      { .len = 1                   , .p_value = &_will_qos         },
      { .len = 1                   , .p_value = &_will_retained    },
      { .len = 0                   , .p_value = NULL               },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  if ( _disconnect_callback )
  {
    para_arr[10].len     = 4;
    para_arr[10].p_value = &this_value;
  }

  return sdep_n(SDEP_CMD_MQTTCONNECT, para_count, para_arr, NULL, &_mqtt_handle);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::connect ( IPAddress ip, uint16_t port, bool cleanSession, uint16_t keepalive_sec)
{
  VERIFY ( tcp.connect(ip, port) );
  _connected = connectBroker(cleanSession, keepalive_sec);

  // close TCP if failed to connect
  if (!_connected) tcp.stop();

  return _connected;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::connect ( const char * host, uint16_t port, bool cleanSession, uint16_t keepalive_sec)
{
  IPAddress ip;
  VERIFY( Feather.hostByName(host, ip) );
  return this->connect(ip, port, cleanSession, keepalive_sec);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::connectSSL(IPAddress ip, uint16_t port, bool cleanSession, uint16_t keepalive_sec)
{
  // Call AdafruitTCP connect
  VERIFY( tcp.connectSSL(ip, port) );
  _connected = connectBroker(cleanSession, keepalive_sec);

  // close TCP if failed to connect
  if (!_connected) tcp.stop();

  return _connected;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::connectSSL ( const char* host, uint16_t port, bool cleanSession, uint16_t keepalive_sec )
{
  IPAddress ip;
  VERIFY( Feather.hostByName(host, ip) );
  return this->connectSSL(ip, port, cleanSession, keepalive_sec);
}

/******************************************************************************/
/*!
    @brief Part of disconnect() that clean up previously allocated resource
*/
/******************************************************************************/
void AdafruitMQTT::disconnect_cleanup(void)
{
  _mqtt_handle = 0;   // clean handle
  tcp.stop();         // Disconnect TCP
  _connected = false;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::disconnect ( void )
{
  // skip if not connected
  if ( !_connected ) return true;

  // Send Disconnect Packet to Broker
  VERIFY( sdep(SDEP_CMD_MQTTDISCONNECT, 4, &_mqtt_handle, NULL, NULL) );

  this->disconnect_cleanup();
  return true;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::publish( UTF8String topic, UTF8String message, uint8_t qos, bool retained )
{
  VERIFY( _connected );

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4           , .p_value = &_mqtt_handle },
      { .len = topic.len   , .p_value = topic.data    },
      { .len = message.len , .p_value = message.data  },
      { .len = 1           , .p_value = &qos          },
      { .len = 1           , .p_value = &retained     },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_MQTTPUBLISH, para_count, para_arr, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::subscribe_internal(const char* topicFilter, uint8_t qos, void* mh, bool copy_topic, void* arg)
{
  VERIFY( _connected && topicFilter != NULL);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4                   , .p_value = &_mqtt_handle },
      { .len = strlen(topicFilter) , .p_value = topicFilter   },
      { .len = 1                   , .p_value = &qos          },
      { .len = 4                   , .p_value = &mh           },
      { .len = 1                   , .p_value = &copy_topic   },
      { .len = 4                   , .p_value = &arg          },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_MQTTSUBSCRIBE, para_count, para_arr, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::subscribe( const char* topicFilter, uint8_t qos, messageHandler_t mh)
{
  return subscribe_internal(topicFilter, qos, (void*) mh, false);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::subscribe_copy( const char* topicFilter, uint8_t qos, messageHandler_t mh)
{
  return subscribe_internal(topicFilter, qos, (void*) mh, true);
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
bool AdafruitMQTT::unsubscribe( const char* topicFilter )
{
  VERIFY( _connected && topicFilter != NULL );

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4                   , .p_value = &_mqtt_handle },
      { .len = strlen(topicFilter) , .p_value = topicFilter   },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_MQTTUNSUBSCRIBE, para_count, para_arr, NULL, NULL);
}


//--------------------------------------------------------------------+
// Callback
//--------------------------------------------------------------------+
/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
void adafruit_mqtt_disconnect_callback(void* p_mqtt)
{
  AdafruitMQTT* pMQTT = (AdafruitMQTT*) p_mqtt;

  // Clean up resource
  pMQTT->disconnect_cleanup();

  if (pMQTT->_disconnect_callback) pMQTT->_disconnect_callback();
}
