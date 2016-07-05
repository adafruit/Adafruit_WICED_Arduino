/**************************************************************************/
/*!
    @file     adafruit_feather.h
    @author   huynguyen, hathach, ktown

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

#ifndef _ADAFRUIT_FEATHER_H_
#define _ADAFRUIT_FEATHER_H_

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

#define WIFI_MAX_SSID_LEN         32
#define WIFI_MAX_PASSPHRASE_LEN   64
#define WIFI_SCAN_RESULT_LEN      52
#define WIFI_MAX_PROFILE          5

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

ASSERT_STATIC( sizeof(wl_ap_info_t) == 52 );

typedef struct ATTR_PACKED
{
    char year[4];        /**< Year         */
    char dash1;          /**< Dash1        */
    char month[2];       /**< Month        */
    char dash2;          /**< Dash2        */
    char day[2];         /**< Day          */
    char T;              /**< T            */
    char hour[2];        /**< Hour         */
    char colon1;         /**< Colon1       */
    char minute[2];      /**< Minute       */
    char colon2;         /**< Colon2       */
    char second[2];      /**< Second       */
    char decimal;        /**< Decimal      */
    char sub_second[6];  /**< Sub-second   */
    char Z;              /**< UTC timezone */

    char nullterm;       // not part of the format, make printf easier
} iso8601_time_t;

ASSERT_STATIC( sizeof(iso8601_time_t) == 28 );

extern "C"
{
//  void adafruit_wifi_connect_callback(void);
  void adafruit_wifi_disconnect_callback(void);
}

class AdafruitFeather : public AdafruitSDEP
{
private:
  char          _boot_version[8];
  char          _fw_version[8];
  char          _sdk_version[8];

  wl_ap_info_t  _ap_info;
  bool          _connected;

  bool          _rootca_init;
  bool          _rootca_default_en;

  struct {
    uint32_t ip;
    uint32_t gateway;
    uint32_t subnet;
  } _static_config;

//  void (*wlan_connect_callback)(void);
  void (*wlan_disconnect_callback)(void);

public:
  AdafruitFeather(void);

  void factoryReset(void);
  void nvmReset(void);

  char const* bootloaderVersion ( void );
  char const* sdkVersion        ( void );
  char const* firmwareVersion   ( void );
  char const* arduinoVersion    ( void );

  // Connect to a WiFi
  bool connect   ( void );
  bool connect   ( const char *ssid );
  bool connect   ( const char *ssid, const char *key, int enc_type = ENC_TYPE_AUTO );

  bool begin     (void) { return this->connect(); }
  bool begin     (const char *ssid ) { return this->connect(ssid); }
  bool begin     ( const char *ssid, const char *key, int enc_type = ENC_TYPE_AUTO ) { return this->connect(ssid, key, enc_type); }

  // Manual config static IP, no parameter will reset the IP to default (DHCP)
  bool config (IPAddress ip, IPAddress gateway, IPAddress subnet);
  bool config (void) { return this->config(INADDR_NONE, INADDR_NONE, INADDR_NONE); }

  bool connected (void) { return _connected; }
  void disconnect(void);

  // Profile functions
  bool      saveConnectedProfile  ( void ); // save currently connected AP
  bool      addProfile            ( char* ssid ); // Open
  bool      addProfile            ( char* ssid, char* key, int enc_type);
  bool      removeProfile         ( char* ssid );
  bool      checkProfile          ( char* ssid ); // Check if profile exists
  void      clearProfiles         ( void );
  char*     profileSSID           ( uint8_t pos);
  int32_t   profileEncryptionType ( uint8_t pos);

  // Info functions
  uint8_t*    macAddress      ( uint8_t *mac );
  uint32_t    localIP         ( void );
  uint32_t    subnetMask      ( void );
  uint32_t    gatewayIP       ( void );

  char*       SSID            ( void );
  int32_t     RSSI            ( void );
  int32_t     encryptionType  ( void );
  uint8_t*    BSSID           ( uint8_t* bssid );

  // Callback
//  void setConnectCallback( void (*fp) (void));
//  {
//    wlan_connect_callback = fp;
//  }

  void setDisconnectCallback( void (*fp) (void))
  {
    wlan_disconnect_callback = fp;
  }

  /* WiFi Commands */
  int scanNetworks(wl_ap_info_t ap_list[], uint8_t max_ap);

  /* Network Commands */
  IPAddress  hostByName( const char* hostname);
  bool       hostByName( const char* hostname, IPAddress& result );
  bool       hostByName( const String &hostname, IPAddress& result ) { return hostByName( hostname.c_str(), result ); }

  // TODO more advanced ping command
  uint32_t   ping                  (char const* host);
  uint32_t   ping                  (IPAddress ip);
  bool       getISO8601Time        (iso8601_time_t* iso8601_time);
  uint32_t   getUtcTime            (void);

  // TLS Root Certification Chain
  bool       useDefaultRootCA      (bool enabled) { _rootca_default_en = enabled; }
  bool       initRootCA            (void);
  bool       addRootCA             (uint8_t const* root_ca, uint16_t len);
  bool       clearRootCA           (void);

  // Enable later when complete
//  err_t startAP(char* ssid, char* passwd);
//  err_t startAP(void);
//  err_t stopAP(void);


  // Helper functions
  void      printVersions   (Print& p = Serial);
  void      printNetwork    (Print& p = Serial);
  void      printEncryption (int32_t enc, Print& p = Serial);

  // Debug functions
  void      dbgThreadlist   (void) { sdep(SDEP_CMD_THREADLIST, 0, NULL, NULL, NULL); }


  /* callback from featherlib */
  friend void adafruit_wifi_disconnect_callback(void);
};

extern AdafruitFeather Feather;

//--------------------------------------------------------------------+
// DEBUG HELPER
//--------------------------------------------------------------------+
#ifndef DBG_ENABLE
#define DBG_ENABLE      0
#endif

#if DBG_ENABLE
  #define DBG_LOCATION()  Serial.printf("%s: %d: \r\n", __PRETTY_FUNCTION__, __LINE__)
  #define DBG_INT(x)      Serial.printf(#x " = %ld\r\n", (uint32_t) (x) )
  #define DBG_HEX(x)      Serial.printf("%s: " #x " = %08lx\r\n", __FUNCTION__, (uint32_t) (x) )
  #define DBG_STR(x)      Serial.printf(#x " = %s\r\n", (char*)(x) )
  #define DBG_BUFFER(buf, n) \
    do {\
      uint8_t* p8 = (uint8_t*) (buf);\
      Serial.print(#buf ": ");\
      for(uint32_t i=0; i<(n); i++) Serial.printf("%02x ", p8[i]);\
      Serial.print("\r\n");\
    }while(0)

  #define calloc_named( name, nelems, elemsize) ({ Serial.printf("[calloc] %s : %d\r\n", name, nelems*elemsize); calloc ( nelems, elemsize ); })
  #define malloc_named( name, size )            ({ Serial.printf("[malloc] %s : %d\r\n", name, size); malloc(size); })
  #define free_named( name, ptr )               ({ Serial.printf("[free  ] %s\r\n"     , name      ); free  (ptr ); })

  #if DBG_ENABLE == 3
  #define DBG_HEAP()      Serial.printf("%s: %d: Heap free: %d\r\n", __FUNCTION__, __LINE__, FEATHERLIB->heap_get_free_size()); delay(5)
  #else
  #define DBG_HEAP()
  #endif
#else
  #define DBG_LOCATION()
  #define DBG_INT(x)
  #define DBG_HEX(x)  #define DBG_STR(x)  #define DBG_HEAP()
  #define DBG_BUFFER(buf, n)

  #define calloc_named( name, nelems, elemsize) calloc ( nelems, elemsize )
  #define malloc_named( name, size )            malloc ( size )
  #define free_named( name, ptr )               free   ( ptr )
#endif

#define malloc_type(object_type)      ((object_type*)malloc_named(#object_type, sizeof(object_type)))
#define calloc_type(object_type)      ((object_type*)calloc_named(#object_type, 1, sizeof(object_type)))


#endif
