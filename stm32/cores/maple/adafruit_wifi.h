/**************************************************************************/
/*!
    @file     adafruit_wifi.h
    @author   huynguyen

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2015, Adafruit Industries (adafruit.com)
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

#ifndef _ADAFRUIT_WIFI_H_
#define _ADAFRUIT_WIFI_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Arduino.h"
#include "adafruit_constants.h"
#include "adafruit_featherlib.h"

/**
 * Maximum packet size for one access point (52 bytes)
 *     1  byte        : length of SSID
 *     32 bytes (max) : SSID
 *     6  bytes       : MAC address
 *     2  bytes       : Receive Signal Strength in dBm
 *     4  bytes       : Maximum data rate in kilobits/s
 *     1  byte        : Network type
 *     4  bytes       : Security type
 *     1  byte        : Radio channel
 *     1  byte        : Radio band
 */
#define AP_INFO_PACKET_SIZE      52

#define WIFI_MAX_SSID_LEN         32
#define WIFI_MAX_PASSPHRASE_LEN   64
#define WIFI_SCAN_RESULT_LEN      52
#define WIFI_MAX_PROFILE          5

enum {
  GET_METHOD    = 0x01,
  POST_METHOD   = 0x02,
};

enum {
  DISABLE       = 0x00,
  ENABLE        = 0x01,
};

typedef struct ATTR_PACKED
{
  char     ssid[WIFI_MAX_SSID_LEN+1];

  uint8_t  bssid[6];
  int16_t  rssi;
  uint32_t max_data_rate;
  uint8_t  network_type;
  int32_t  security;
  uint8_t  channel;
  uint8_t  band_2_4ghz;
} wl_ap_info_t;

ASSERT_STATIC(sizeof(wl_ap_info_t) == 52);

typedef uint16_t sdep_err_t;

extern "C"
{
  void http_rx_callback (uint8_t* data, uint16_t data_length, uint16_t avail);
  void mqtt_evt_callback(mqtt_evt_opcode_t event, uint16_t len, uint8_t* data);

  /* Callback prototypes */
  typedef void (*ada_http_callback)(uint8_t* data, uint16_t data_length, uint16_t avail);
  typedef void (*ada_mqtt_callback)(mqtt_evt_opcode_t event, uint16_t len, uint8_t* data);
}

class AdafruitFeather
{
private:
  wl_ap_info_t  _ap_info;
  bool          _connected;

  ada_http_callback  ada_http_rx_callback;
  ada_mqtt_callback  ada_mqtt_evt_callback;

public:
  AdafruitFeather(void);

  sdep_err_t randomNumber(uint32_t* random32bit);
  sdep_err_t factoryReset(void);

  // Connect to a WiFi
  bool   connect           ( void );
  bool   connect           ( const char *ssid );
  bool   connect           ( const char *ssid, const char *key, int enc_type = ENC_TYPE_AUTO );
  //bool connect           ( const char *ssid, uint8_t key_idx, const char* key );

  //bool connect           ( const String &ssid, uint8_t key_idx, const String &key ) { return connect(ssid.c_str(), key_idx, key.c_str()); }
  //bool   connect           ( const String &ssid ) { return connect(ssid.c_str()); }
  //bool   connect           ( const String &ssid, const String &key, int enc_type = ENC_TYPE_AUTO ) { return connect(ssid.c_str(), key.c_str(), enc_type); }

  uint8_t   *macAddress     ( uint8_t *mac );

  /* WiFi Commands */
  int scanNetworks(wl_ap_info_t ap_list[], uint8_t max_ap);

  sdep_err_t connectAP(char const* ssid, char const* passwd);
  sdep_err_t disconnectAP(void);

  sdep_err_t startAP(char* ssid, char* passwd);
  sdep_err_t startAP(void);
  sdep_err_t stopAP(void);
  sdep_err_t getRSSI(int8_t *rssi);
  sdep_err_t getIPAddress(uint32_t *addr);

  /* Network Commands */
  sdep_err_t ping(char* ip_address_str, uint8_t* response_time);
  sdep_err_t ping(uint8_t* ip_address, uint8_t* response_time);
  sdep_err_t dnsLookup(const char* dns, uint8_t* ipv4_address);
  sdep_err_t getTime(char* iso8601_time);
  sdep_err_t httpGetUri(char* uri, uint16_t* length, uint8_t* response);
  sdep_err_t httpPost(char* uri, uint16_t* length, uint8_t* response);

  sdep_err_t httpRequest(const char* url, const char* content, uint8_t method, uint32_t buffer_length, uint8_t* buffer);
  sdep_err_t httpsRequest(const char* url, const char* ca_cert, const char* content, uint8_t method, uint32_t buffer_length, uint8_t* buffer);
  sdep_err_t httpRequestWithCallback(const char* url, const char* content, uint8_t method);
  sdep_err_t httpsRequestWithCallback(const char* url, const char* ca_cert, const char* content, uint8_t method);
  void       addHttpDataReceivedCallBack(ada_http_callback ada_httpCallback = NULL);

//  /* DEBUG Commands */
//  sdep_err_t stackDump();
//  sdep_err_t stackSize();
//  sdep_err_t heapDump();
//  sdep_err_t heapSize();
//  sdep_err_t threadList();
//
//  /* SPI Flash Commands */
//  sdep_err_t sflashFormat();
//  sdep_err_t sflashList();

  /* MQTT Commands */
  sdep_err_t mqttLastWill(bool isOnlineTopic, char* topic, char* value, uint8_t qos, uint8_t retain);
  sdep_err_t mqttGenerateRandomID(char* clientID, uint8_t length);
  sdep_err_t mqttConnect(char* host, uint16_t port, char* clientID, char* username, char* password);
  sdep_err_t mqttTLSConnect(char* host, uint16_t port, char* clientID, char *username, char* password, const char* ca_cert);
  sdep_err_t mqttDisconnect(void);
  sdep_err_t mqttPublish(char* topic, char* value, uint8_t qos, uint8_t retain);
  sdep_err_t mqttSubscribe(char* topic, uint8_t qos);
  sdep_err_t mqttUnsubscribe(char* topic);
  void       addMqttCallBack(ada_mqtt_callback ada_mqttCallback = NULL);

  /* IRQ Commands */
  sdep_err_t irqRead(uint16_t* response_length, uint8_t* response);
  sdep_err_t irqCount(uint16_t* n_items);
  sdep_err_t irqAvailable(uint16_t* n_available);
  sdep_err_t irqClear(void);

  /* callback from featherlib */
  friend void http_rx_callback (uint8_t* data, uint16_t data_length, uint16_t avail);
  friend void mqtt_evt_callback(mqtt_evt_opcode_t event, uint16_t len, uint8_t* data);
};

extern AdafruitFeather feather;

#endif
