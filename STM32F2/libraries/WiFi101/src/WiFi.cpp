/*
  WiFi.cpp - Library for Arduino Wifi shield.
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

#include "WiFi101.h"
#include "adafruit_wifi.h"

// TODO remove later (using malloc/free to save memory)
#define WIFI_MAX_SCAN_NUM   20
static wl_ap_info_t wifi_scan_result[WIFI_MAX_SCAN_NUM];

WiFiClass::WiFiClass()
{
	_mode    = WL_RESET_MODE;
	_status  = WL_IDLE_STATUS;

	_submask = 0;
	_gateway = 0;

	uint8_t fw_version[4] = { U32_TO_U8S_BE(FEATHERLIB->firmware_version) };
	sprintf(_version, "%d.%d.%d", fw_version[0], fw_version[1], fw_version[2]);

	memclr(&_ap_info, sizeof(_ap_info));
}

char* WiFiClass::firmwareVersion()
{
	return _version;
}

uint8_t WiFiClass::begin()
{
	// Connect to router using saved profiles
  _status = (ERROR_NONE == FEATHERLIB->sdep_execute(SDEP_CMD_CONNECT, 0, NULL, NULL, NULL)) ? WL_CONNECTED : WL_CONNECT_FAILED;

	return _status;
}

uint8_t WiFiClass::begin(const char *ssid)
{
  if (ssid == NULL || ssid == "") return WL_NO_SSID_AVAIL;

  uint16_t resp_len = sizeof(wl_ap_info_t);
  uint16_t err = FEATHERLIB->sdep_execute(SDEP_CMD_CONNECT,
                                          strlen(ssid) + 1, ssid,
                                          &resp_len, &_ap_info);

  _status = (err == ERROR_NONE) ? WL_CONNECTED : WL_CONNECT_FAILED;
	return _status;
}

uint8_t WiFiClass::begin(const char *ssid, const char *key, int enc_type)
{
  if (ssid == NULL || ssid == "") return WL_NO_SSID_AVAIL;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid) + 1 , .p_value = ssid      },
      { .len = strlen(key)  + 1 , .p_value = key       },
      { .len = 4                , .p_value = &enc_type },
  };

  uint16_t resp_len = sizeof(wl_ap_info_t);
  uint16_t err = FEATHERLIB->sdep_execute_n(SDEP_CMD_CONNECT,
                                            sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                            &resp_len, &_ap_info);

  _status = (err == ERROR_NONE) ? WL_CONNECTED : WL_CONNECT_FAILED;
	return _status;
}

uint8_t WiFiClass::beginAP(char *ssid)
{
	return beginAP(ssid, 1);
}

uint8_t WiFiClass::beginAP(char *ssid, uint8_t channel)
{
  return 0;
#if 0
	tstrM2MAPConfig strM2MAPConfig;

	if (!_init) {
		init();
	}

	// Enter Access Point mode:
	memset(&strM2MAPConfig, 0x00, sizeof(tstrM2MAPConfig));
	strcpy((char *)&strM2MAPConfig.au8SSID, ssid);
	strM2MAPConfig.u8ListenChannel = channel;
	strM2MAPConfig.u8SecType = M2M_WIFI_SEC_OPEN;
	strM2MAPConfig.au8DHCPServerIP[0] = 0xC0; /* 192 */
	strM2MAPConfig.au8DHCPServerIP[1] = 0xA8; /* 168 */
	strM2MAPConfig.au8DHCPServerIP[2] = 0x01; /* 1 */
	strM2MAPConfig.au8DHCPServerIP[3] = 0x01; /* 1 */
	if (m2m_wifi_enable_ap(&strM2MAPConfig) < 0) {
		_status = WL_CONNECT_FAILED;
		return _status;
	}
	_status = WL_CONNECTED;
	_mode = WL_AP_MODE;

	memset(_ssid, 0, M2M_MAX_SSID_LEN);
	memcpy(_ssid, ssid, strlen(ssid));
	m2m_memcpy((uint8 *)&_localip, (uint8 *)&strM2MAPConfig.au8DHCPServerIP[0], 4);
	_submask = 0x00FFFFFF;
	_gateway = _localip;

	// WiFi led ON.
	m2m_periph_gpio_set_val(M2M_PERIPH_GPIO15, 0);

	return _status;
#endif

}

uint8_t WiFiClass::beginProvision(char *ssid, char *url)
{
	return beginProvision(ssid, url, 1);
}

uint8_t WiFiClass::beginProvision(char *ssid, char *url, uint8_t channel)
{
  return 0;

#if 0
	tstrM2MAPConfig strM2MAPConfig;

	if (!_init) {
		init();
	}

	// Enter Provision mode:
	memset(&strM2MAPConfig, 0x00, sizeof(tstrM2MAPConfig));
	strcpy((char *)&strM2MAPConfig.au8SSID, ssid);
	strM2MAPConfig.u8ListenChannel = channel;
	strM2MAPConfig.u8SecType = M2M_WIFI_SEC_OPEN;
	strM2MAPConfig.u8SsidHide = SSID_MODE_VISIBLE;
	strM2MAPConfig.au8DHCPServerIP[0] = 0xC0; /* 192 */
	strM2MAPConfig.au8DHCPServerIP[1] = 0xA8; /* 168 */
	strM2MAPConfig.au8DHCPServerIP[2] = 0x01; /* 1 */
	strM2MAPConfig.au8DHCPServerIP[3] = 0x01; /* 1 */

	if (m2m_wifi_start_provision_mode((tstrM2MAPConfig *)&strM2MAPConfig, url, 1) < 0) {
		_status = WL_CONNECT_FAILED;
		return _status;
	}
	_status = WL_CONNECTED;
	_mode = WL_PROV_MODE;

	memset(_ssid, 0, M2M_MAX_SSID_LEN);
	memcpy(_ssid, ssid, strlen(ssid));
	m2m_memcpy((uint8 *)&_localip, (uint8 *)&strM2MAPConfig.au8DHCPServerIP[0], 4);
	_submask = 0x00FFFFFF;
	_gateway = _localip;

	// WiFi led ON.
	m2m_periph_gpio_set_val(M2M_PERIPH_GPIO15, 0);

	return _status;
#endif
}

uint32_t WiFiClass::provisioned()
{
	if (_mode == WL_STA_MODE) {
		return 1;
	}
	else {
		return 0;
	}
}

// Configuring static IP instead of using DHCP
void WiFiClass::config(IPAddress local_ip)
{
	_submask = 0;
	_gateway = 0;
}

void WiFiClass::config(IPAddress local_ip, IPAddress dns_server)
{
#if 0
	tstrM2MIPConfig conf;

	conf.u32DNS = (uint32_t)dns_server;
	conf.u32Gateway = 0;
	conf.u32StaticIP = (uint32_t)local_ip;
	conf.u32SubnetMask = 0;
	m2m_wifi_set_static_ip(&conf);
#endif
	_submask = 0;
	_gateway = 0;
}

void WiFiClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
#if 0
	tstrM2MIPConfig conf;

	conf.u32DNS = (uint32_t)dns_server;
	conf.u32Gateway = (uint32_t)gateway;
	conf.u32StaticIP = (uint32_t)local_ip;
	conf.u32SubnetMask = 0;
	m2m_wifi_set_static_ip(&conf);
	_localip = conf.u32StaticIP;
	_submask = 0;
	_gateway = conf.u32Gateway;
#endif
}

void WiFiClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
#if 0
	tstrM2MIPConfig conf;

	conf.u32DNS = (uint32_t)dns_server;
	conf.u32Gateway = (uint32_t)gateway;
	conf.u32StaticIP = (uint32_t)local_ip;
	conf.u32SubnetMask = (uint32_t)subnet;
	m2m_wifi_set_static_ip(&conf);
	_localip = conf.u32StaticIP;
	_submask = conf.u32SubnetMask;
	_gateway = conf.u32Gateway;
#endif
}

void WiFiClass::disconnect()
{
  if ( _status != WL_CONNECTED ) return;

  FEATHERLIB->sdep_execute(SDEP_CMD_DISCONNECT, 0, NULL, NULL, NULL);
  _status = WL_DISCONNECTED;
}

uint8_t *WiFiClass::macAddress(uint8_t *mac)
{
	FEATHERLIB->sdep_execute(SDEP_CMD_GET_MAC_ADDRESS, 0, NULL, NULL, mac);
	return mac;
}

uint32_t WiFiClass::localIP()
{
  if ( _status != WL_CONNECTED ) return 0;

  uint8_t interface   = WIFI_INTERFACE_STATION;
  uint32_t ip_address = 0;

  FEATHERLIB->sdep_execute(SDEP_CMD_GET_IPV4_ADDRESS, 1, &interface, NULL, &ip_address);
	return ip_address;
}

uint32_t WiFiClass::subnetMask()
{
  if ( _status != WL_CONNECTED ) return 0;

  uint8_t interface   = WIFI_INTERFACE_STATION;
  uint32_t subnet = 0;

  FEATHERLIB->sdep_execute(SDEP_CMD_GET_NETMASK, 1, &interface, NULL, &subnet);
  return subnet;
}

uint32_t WiFiClass::gatewayIP()
{
  if ( _status != WL_CONNECTED ) return 0;

  uint8_t interface   = WIFI_INTERFACE_STATION;
  uint32_t ip_address = 0;

  FEATHERLIB->sdep_execute(SDEP_CMD_GET_GATEWAY_ADDRESS, 1, &interface, NULL, &ip_address);
  return ip_address;
}

char* WiFiClass::SSID()
{
  if ( _status != WL_CONNECTED ) return NULL;
	return _ap_info.ssid;
}

uint8_t* WiFiClass::BSSID(uint8_t* bssid)
{
  memcpy(bssid, _ap_info.bssid, 6);
  return bssid;
}

uint32_t WiFiClass::encryptionType()
{
  return _ap_info.security;
}

int32_t WiFiClass::RSSI()
{
  if ( _status != WL_CONNECTED ) return 0;

  int32_t rssi;
  FEATHERLIB->sdep_execute(SDEP_CMD_WIFI_GET_RSSI, 0, NULL, NULL, &rssi);

  return rssi;
}

int8_t WiFiClass::scanNetworks()
{
  uint16_t length = sizeof(wl_ap_info_t) * WIFI_MAX_SCAN_NUM;
  VERIFY( ERROR_NONE == FEATHERLIB->sdep_execute(SDEP_CMD_SCAN, 0, NULL, &length, wifi_scan_result), 0);

  return length/sizeof(wl_ap_info_t);
}

char* WiFiClass::SSID(uint8_t pos)
{
  VERIFY(pos < WIFI_MAX_SCAN_NUM, NULL);

  wl_ap_info_t* p_scan = &wifi_scan_result[pos];

  return p_scan->ssid;
}

int32_t WiFiClass::RSSI(uint8_t pos)
{
  VERIFY(pos < WIFI_MAX_SCAN_NUM, NULL);

  wl_ap_info_t* p_scan = &wifi_scan_result[pos];

  return p_scan->rssi;
}

uint32_t WiFiClass::encryptionType(uint8_t pos)
{
  VERIFY(pos < WIFI_MAX_SCAN_NUM, NULL);

  wl_ap_info_t* p_scan = &wifi_scan_result[pos];

  return p_scan->security;
}

uint8_t WiFiClass::status()
{
	return _status;
}

int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult)
{
  if ( _status != WL_CONNECTED ) return NULL;

  uint32_t ip_addr = 0;
  int err = FEATHERLIB->sdep_execute(SDEP_CMD_DNSLOOKUP, strlen(aHostname), aHostname, NULL, &ip_addr);

  if( ERROR_NONE != err ) return (-err);

  aResult = ip_addr;

  return 1;
}

bool WiFiClass::addProfile(char* ssid)
{
  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid) + 1 , .p_value = ssid      },
  };

  // TODO check case when read bytes < size
  return (ERROR_NONE == FEATHERLIB->sdep_execute_n(SDEP_CMD_WIFI_PROFILE_ADD,
                                                   sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                                   NULL, NULL)) ? true : false;
}

bool WiFiClass::addProfile(char* ssid, char* key, int enc_type)
{
  if ( enc_type == ENC_TYPE_AUTO )
  {
    Serial.println("Encryption cannot be AUTO for now");
    return false;
  }

  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid) + 1 , .p_value = ssid      },
      { .len = strlen(key)  + 1 , .p_value = key       },
      { .len = 4                , .p_value = &enc_type },
  };

  // TODO check case when read bytes < size
  return (ERROR_NONE == FEATHERLIB->sdep_execute_n(SDEP_CMD_WIFI_PROFILE_ADD,
                                                   sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                                   NULL, NULL)) ? true : false;
}

bool WiFiClass::delProfile(char* ssid)
{
  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid) + 1 , .p_value = ssid },
  };

  // TODO check case when read bytes < size
  return (ERROR_NONE == FEATHERLIB->sdep_execute_n(SDEP_CMD_WIFI_PROFILE_DEL,
                                                   sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                                   NULL, NULL)) ? true : false;
}

void WiFiClass::clearProfile(void)
{
  FEATHERLIB->sdep_execute(SDEP_CMD_WIFI_PROFILE_CLEAR, 0, NULL, NULL, NULL);
}

bool WiFiClass::checkProfile(char* ssid)
{
  bool result = false;

  return (ERROR_NONE == FEATHERLIB->sdep_execute(SDEP_CMD_WIFI_PROFILE_CHECK,
                                                 strlen(ssid)+1, ssid,
                                                 NULL, &result) ) ? result : false ;
}

WiFiClass WiFi;
