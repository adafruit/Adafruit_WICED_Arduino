/**************************************************************************/
/*!
    @file     adafruit_featherap.cpp
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

#include "adafruit_featherap.h"

typedef enum
{
  SOFTAP_EVENT_UNKNOWN,
  SOFTAP_EVENT_JOINED,
  SOFTAP_EVENT_LEAVE,
} softap_event_t;

AdafruitFeatherAP FeatherAP;

/**
 *
 */
void AdafruitFeatherAP::clear(void)
{
  _ip = _gateway = _subnet = 0;
  _channel = 0;

  _client_count = 0;
  memclr(_client_maclist, sizeof(_client_maclist));

  _join_callback = _leave_callback = NULL;
}

/**
 * Contructor
 */
AdafruitFeatherAP::AdafruitFeatherAP(void)
{
  this->clear();
}

/**
 *
 * @param ip
 * @param gateway
 * @param subnet
 * @param channel
 * @return
 */
bool AdafruitFeatherAP::begin(IPAddress ip, IPAddress gateway, IPAddress subnet, uint8_t channel)
{
  _ip      = (uint32_t) ip;
  _gateway = gateway;
  _subnet  = subnet;
  _channel = channel;
}

/******************************************************************************/
/*!
    @brief  Starts AP mode on the module, causing the module to advertise a new
            Wireless network and SSID, etc.

    @param[in]      ssid    SSID of Wi-Fi access point

    @param[in]      passwd  Password of Wi-Fi access point

    SDEP Parameter
    0. SSID
    1. Password
    2. Encryption type
    3. Config IP
    4. Config Gateway
    5. Config Subnet
    6. Channel

    @return true if success
*/
/******************************************************************************/
bool AdafruitFeatherAP::start(const char *ssid, const char *key, int enc_type)
{
  VERIFY( (ssid != NULL) && strlen(ssid) > 0);
  VERIFY( (key != NULL) || (enc_type == ENC_TYPE_OPEN));
  VERIFY( (_ip > 0) && (_gateway > 0) && (_subnet > 0) );

  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid)          , .p_value = ssid      },
      { .len = key ? strlen(key) : 0 , .p_value = key       },
      { .len = 4                     , .p_value = &enc_type },

      { .len = 4                     , .p_value = &_ip      },
      { .len = 4                     , .p_value = &_gateway },
      { .len = 4                     , .p_value = &_subnet  },
      { .len = 1                     , .p_value = &_channel },
  };

  return sdep_n(SDEP_CMD_SOFTAP_START, arrcount(para_arr), para_arr, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Stop AP mode
*/
/******************************************************************************/
void AdafruitFeatherAP::stop(void)
{
//  return FEATHERLIB->sdep_execute(SDEP_CMD_APSTOP, 0, NULL, NULL, NULL);
}


const uint8_t* AdafruitFeatherAP::clientMAC(uint8_t id)
{
  return  ( id < _client_count) ? _client_maclist[id] : NULL;
}

/******************************************************************************/
/*!
    @brief  Get RSSI of an connected client based on its index

    @param[in]      id    Client ID

    SDEP Parameter
    0. Client MAC

    @return SSID of client
*/
/******************************************************************************/
int32_t AdafruitFeatherAP::clientRSSI(uint8_t id)
{
  if ( id >= _client_count ) return 0;

  int32_t rssi = 0;

  (void) sdep(SDEP_CMD_SOFTAP_CLIENT_RSSI, 6, _client_maclist[id], NULL, &rssi);

  return rssi;
}

/**
 *
 * @param event
 * @param mac
 */
void AdafruitFeatherAP::featherlib_event_callback(uint32_t event, const uint8_t mac[6] )
{
  if ( SOFTAP_EVENT_JOINED == event && _client_count < SOFTAP_MAX_CLIENT)
  {
    memcpy(_client_maclist[_client_count++], mac, 6);
    if(_join_callback) _join_callback(mac);
  }

  if ( SOFTAP_EVENT_LEAVE == event)
  {
    // Find the corresponding client and shift the maclist
    for(int i=0; i<_client_count; i++)
    {
      if ( 0 == memcmp(mac, _client_maclist[i], 6) )
      {
        if ( i < _client_count-1)
        {
          memmove(_client_maclist[i], _client_maclist[i+1], 6*(_client_count-1-i) );
        }
        memclr(_client_maclist[_client_count-1], 6);

        break;
      }
    }

    _client_count--;
    if (_leave_callback) _leave_callback(mac);
  }
}

/**
 *
 * @param fp
 */
void AdafruitFeatherAP::setJoinCallback( void(*fp)(const uint8_t[6]))
{
  _join_callback = fp;
}

/**
 *
 * @param fp
 */
void AdafruitFeatherAP::setLeaveCallback( void(*fp)(const uint8_t[6]))
{
  _leave_callback = fp;
}

/**
 *
 * @param event
 * @param mac
 */
void adafruit_softap_event_callback(uint32_t event, const uint8_t mac[6] )
{
  FeatherAP.featherlib_event_callback(event, mac);
}
