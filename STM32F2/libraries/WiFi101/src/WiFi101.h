/*
  WiFi.h - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>

//extern "C" {
//	#include "driver/include/m2m_wifi.h"
//	#include "socket/include/socket.h"
//}

#include "WiFiClient.h"
//#include "WiFiServer.h"

#define WIFI_FIRMWARE_REQUIRED    "0.1.0"
#define WIFI_MAX_SSID_LEN         32
#define WIFI_MAX_PASSPHRASE_LEN   64
#define WIFI_SCAN_RESULT_LEN      52


#define SHARED_ENABLED  0x00008000
#define WPA_SECURITY    0x00200000
#define WPA2_SECURITY   0x00400000
#define WPS_ENABLED     0x10000000

#define OPEN_AUTH                   0x0000
#define SHARED_AUTH                 0x0001
#define WEP_ENABLED                 0x0001
#define TKIP_ENABLED                0x0002
#define AES_ENABLED                 0x0004
#define WSEC_SWFLAG                 0x0008

typedef struct ATTR_PACKED
{
  char     ssid[WIFI_MAX_SSID_LEN+1];

  uint8_t  bssid[6];
  int16_t  rssi;
  uint32_t max_data_rate;
  uint8_t  network_type;
  uint32_t security;
  uint8_t  channel;
  uint8_t  band_2_4ghz;
} wl_ap_info_t;

ASSERT_STATIC(sizeof(wl_ap_info_t) == 52);

typedef enum {
	WL_NO_SHIELD = 255,
	WL_IDLE_STATUS = 0,
	WL_NO_SSID_AVAIL,
	WL_SCAN_COMPLETED,
	WL_CONNECTED,
	WL_CONNECT_FAILED,
	WL_CONNECTION_LOST,
	WL_DISCONNECTED
} wl_status_t;

typedef enum
{
  ENC_TYPE_AUTO       = -1,                                               /**< Auto detection                         */
  ENC_TYPE_OPEN       = 0,                                                /**< Open security                           */

  ENC_TYPE_WEP        = WEP_ENABLED,                                      /**< WEP Security with open authentication   */
  ENC_TYPE_WEP_SHARED = ( WEP_ENABLED | SHARED_ENABLED ),                 /**< WEP Security with shared authentication */

  ENC_TYPE_WPA_TKIP   = ( WPA_SECURITY  | TKIP_ENABLED ),                 /**< WPA Security with TKIP                  */
  ENC_TYPE_WPA_AES    = ( WPA_SECURITY  | AES_ENABLED ),                  /**< WPA Security with AES                   */

  ENC_TYPE_WPA2_AES   = ( WPA2_SECURITY | AES_ENABLED ),                  /**< WPA2 Security with AES                  */
  ENC_TYPE_WPA2_TKIP  = ( WPA2_SECURITY | TKIP_ENABLED ),                 /**< WPA2 Security with TKIP                 */
  ENC_TYPE_WPA2_MIXED = ( WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED ),   /**< WPA2 Security with AES & TKIP           */

  ENC_TYPE_WPS_OPEN   = ( WPS_ENABLED ),                                   /**< WPS with open security                  */
  ENC_TYPE_WPS_SECURE = ( WPS_ENABLED | AES_ENABLED),                      /**< WPS with AES security                   */
} wl_enc_type_t;

typedef enum {
	WL_RESET_MODE = 0,
	WL_STA_MODE,
	WL_PROV_MODE,
	WL_AP_MODE
} wl_mode_t;

class WiFiClass
{
public:
  wl_ap_info_t _ap_info;

	uint32_t _submask;
	uint32_t _gateway;
	uint32_t _resolve;

	wl_mode_t _mode;
	wl_status_t _status;

#if 0
	WiFiClient *_client[TCP_SOCK_MAX];
#endif

	WiFiClass();

	int init() { return 1; }
	
	char* firmwareVersion();

	/* Start Wifi connection with WPA/WPA2 encryption.
	 *
	 * param ssid: Pointer to the SSID string.
	 * param key: Key input buffer.
	 */
	uint8_t begin();
	uint8_t begin(const char *ssid);
//	uint8_t begin(const char *ssid, uint8_t key_idx, const char* key);
	uint8_t begin(const char *ssid, const char *key, int enc_type = ENC_TYPE_AUTO);

	uint8_t begin(const String &ssid) { return begin(ssid.c_str()); }
//	uint8_t begin(const String &ssid, uint8_t key_idx, const String &key) { return begin(ssid.c_str(), key_idx, key.c_str()); }
	uint8_t begin(const String &ssid, const String &key, int enc_type = ENC_TYPE_AUTO) { return begin(ssid.c_str(), key.c_str(), enc_type); }

	bool addProfile(char* ssid); // open
	bool addProfile(char* ssid, char* key, int enc_type = ENC_TYPE_AUTO);
	bool delProfile(char* ssid);
	bool checkProfile(char* ssid); // check if profile is existed
	void clearProfile(void);


	/* Start Wifi in Access Point, with open security.
	 * Only one client can connect to the AP at a time.
	 *
	 * param ssid: Pointer to the SSID string.
	 * param channel: Wifi channel to use. Valid values are 1-12.
	 */
	uint8_t beginAP(char *ssid);
	uint8_t beginAP(char *ssid, uint8_t channel);

	uint8_t beginProvision(char *ssid, char *url);
	uint8_t beginProvision(char *ssid, char *url, uint8_t channel);

	uint32_t provisioned();

	void config(IPAddress local_ip);
	void config(IPAddress local_ip, IPAddress dns_server);
	void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway);
	void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);

	void disconnect();

	uint8_t *macAddress(uint8_t *mac);

	uint32_t localIP();
	uint32_t subnetMask();
	uint32_t gatewayIP();

	char* SSID();
	int32_t RSSI();
	uint32_t encryptionType();
	uint8_t* BSSID(uint8_t* bssid);

	int8_t scanNetworks();
	char* SSID(uint8_t pos);
	int32_t RSSI(uint8_t pos);
	uint32_t encryptionType(uint8_t pos);

	uint8_t status();

	int hostByName(const char* hostname, IPAddress& result);
	int hostByName(const String &hostname, IPAddress& result) { return hostByName(hostname.c_str(), result); }

	void refresh(void) {}

private:
	char _version[9];
};

extern WiFiClass WiFi;

#endif /* WIFI_H */
