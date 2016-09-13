/**************************************************************************/
/*!
    @file     adafruit_featherap.h
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

#ifndef _ADAFRUIT_FEATHERAP_H_
#define _ADAFRUIT_FEATHERAP_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <Arduino.h>
#include <IPAddress.h>

#include "adafruit_constants.h"
#include "adafruit_featherlib.h"
#include "adafruit_utf8string.h"
#include "adafruit_sdep.h"
#include "adafruit_tcp.h"
#include "adafruit_tcp_server.h"
#include "adafruit_udp.h"

// Limit by SDK due to RAM restriction
#define SOFTAP_MAX_CLIENT   5

extern "C"
{
  void adafruit_softap_event_callback(uint32_t event, const uint8_t mac[6] );
}

class AdafruitFeatherAP : public AdafruitSDEP
{
private:
  uint32_t _ip;
  uint32_t _gateway;
  uint32_t _subnet;
  uint8_t  _channel;

  char const * _ssid;
  uint32_t     _enc_type;

  bool    _started;

  uint8_t  _client_count;
  uint8_t  _client_maclist[SOFTAP_MAX_CLIENT][6];

  void (*_join_callback)(const uint8_t mac[6]);
  void (*_leave_callback)(const uint8_t mac[6]);

  void featherlib_event_callback(uint32_t event, const uint8_t mac[6] );


  void clear(void);

public:
  AdafruitFeatherAP(void);

  bool begin (IPAddress ip, IPAddress gateway, IPAddress subnet, uint8_t channel = 0);

  bool start(const char *ssid, const char *key, int enc_type = ENC_TYPE_WPA2_AES );
  bool start(const char *ssid ) { return this->start(ssid, NULL, ENC_TYPE_OPEN); }
  void stop();

  bool started (void);
  uint8_t*       macAddress ( uint8_t *mac );

  uint8_t        clientNum  (void) { return _client_count; }
  const uint8_t* clientMAC  (uint8_t id);
  int32_t        clientRSSI (uint8_t id);

  void setJoinCallback( void(*fp)(const uint8_t[6]));
  void setLeaveCallback( void(*fp)(const uint8_t[6]));

  // Helper functions
  void      printNetwork    (Print& p = Serial);

  /* callback from featherlib */
  friend void adafruit_softap_event_callback(uint32_t event, const uint8_t mac[6] );
};

extern AdafruitFeatherAP FeatherAP;

#endif /* _ADAFRUIT_FEATHERAP_H_ */
