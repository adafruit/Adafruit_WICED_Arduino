/**************************************************************************/
/*!
    @file     adafruit_feather.cpp
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

#include "adafruit_featherlib.h"
#include "adafruit_feather.h"
#include "itoa.h"
#include <string.h>
#include <stdlib.h>

extern "C"
{
  extern adafruit_arduino_t const adafruit_arduino;
}

AdafruitFeather Feather;

/******************************************************************************/
/*!
    @brief Instantiates a new instance of the AdafruitFeather class
*/
/******************************************************************************/
AdafruitFeather::AdafruitFeather(void)
{
  _connected = false;
  memclr(&_ap_info, sizeof(wl_ap_info_t));

  ada_http_rx_callback = NULL;
  ada_http_rx_callback = NULL;
  wlan_disconnect_callback = NULL;

  uint8_t boot_version[4] = { U32_TO_U8S_BE(FEATHERLIB->firmware_version) };
	sprintf(_boot_version, "%d.%d.%d", boot_version[0], boot_version[1], boot_version[2]);

  uint8_t fw_version[4] = { U32_TO_U8S_BE(FEATHERLIB->firmware_version) };
	sprintf(_fw_version, "%d.%d.%d", fw_version[0], fw_version[1], fw_version[2]);

	uint8_t sdk_version[4] = { U32_TO_U8S_BE(FEATHERLIB->sdk_version) };
	sprintf(_sdk_version, "%d.%d.%d", sdk_version[0], sdk_version[1], sdk_version[2]);
}

/******************************************************************************/
/*!
    @brief  Performs a factory reset: Erase Arduino code and Set NVM to defaults
*/
/******************************************************************************/
void AdafruitFeather::factoryReset(void)
{
  sdep(SDEP_CMD_FACTORYRESET, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Set NVM to defaults
*/
/******************************************************************************/
void AdafruitFeather::nvmReset(void)
{
  sdep(SDEP_CMD_NVM_RESET, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Get bootloader version in string format
*/
/******************************************************************************/
char const* AdafruitFeather::bootloaderVersion (void)
{
  return this->_boot_version;
}

/******************************************************************************/
/*!
    @brief  Get sdk version in string format
*/
/******************************************************************************/
char const* AdafruitFeather::sdkVersion (void)
{
  return this->_sdk_version;
}

/******************************************************************************/
/*!
    @brief  Get firmware version in string format
*/
/******************************************************************************/
char const* AdafruitFeather::firmwareVersion (void)
{
  return this->_fw_version;
}

/******************************************************************************/
/*!
    @brief  Get Arduino Library in string format
*/
/******************************************************************************/
char const* AdafruitFeather::arduinoVersion ( void )
{
  return adafruit_arduino.version_str;
}


/******************************************************************************/
/*!
    @brief  Return a 32-bit random number

    @param[out]     random32bit       The returned 32-bit random number

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
err_t AdafruitFeather::randomNumber(uint32_t* random32bit)
{
  sdep(SDEP_CMD_RANDOMNUMBER, 0, NULL, NULL, (uint8_t*)random32bit);
  return _errno;
}

/******************************************************************************/
/*!
    @brief Connect using saved profiles
*/
/******************************************************************************/
bool AdafruitFeather::connect(void)
{
  // TODO get ap info
  _connected = sdep(SDEP_CMD_CONNECT, 0, NULL, NULL, NULL);
	return _connected;
}

/******************************************************************************/
/*!
    @brief Connect to an open SSID
*/
/******************************************************************************/
bool AdafruitFeather::connect(const char *ssid)
{
  VERIFY(ssid != NULL && strlen(ssid) > 0);

  uint16_t resp_len = sizeof(wl_ap_info_t);
  _connected = sdep(SDEP_CMD_CONNECT, strlen(ssid), ssid,
                                      &resp_len, &_ap_info);
	return _connected;
}

/******************************************************************************/
/*!
    @brief Connect to an encrypted SSID
*/
/******************************************************************************/
bool AdafruitFeather::connect(const char *ssid, const char *key, int enc_type)
{
  VERIFY(ssid != NULL && strlen(ssid) > 0);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid), .p_value = ssid      },
      { .len = strlen(key) , .p_value = key       },
      { .len = 4           , .p_value = &enc_type },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  uint16_t resp_len = sizeof(wl_ap_info_t);
  _connected = sdep_n(SDEP_CMD_CONNECT, para_count, para_arr, &resp_len, &_ap_info);

	return _connected;
}

/******************************************************************************/
/*!
    @brief Get the MAC Address of the WiFi
*/
/******************************************************************************/
uint8_t* AdafruitFeather::macAddress(uint8_t *mac)
{
	sdep(SDEP_CMD_GET_MAC_ADDRESS, 0, NULL, NULL, mac);
	return mac;
}

/******************************************************************************/
/*!
    @brief Get local IP
*/
/******************************************************************************/
uint32_t AdafruitFeather::localIP (void)
{
  uint8_t interface = WIFI_INTERFACE_STATION;
  uint32_t ipv4 = 0;

  sdep(SDEP_CMD_GET_IPV4_ADDRESS, 1, &interface, NULL, &ipv4);
	return ipv4;
}

/******************************************************************************/
/*!
    @brief Get subnet mask
*/
/******************************************************************************/
uint32_t AdafruitFeather::subnetMask (void)
{
  uint8_t interface = WIFI_INTERFACE_STATION;
  uint32_t subnet = 0;

  sdep(SDEP_CMD_GET_NETMASK, 1, &interface, NULL, &subnet);
  return subnet;
}

/******************************************************************************/
/*!
    @brief Get gateway IP
*/
/******************************************************************************/
uint32_t AdafruitFeather::gatewayIP (void)
{
  uint8_t interface   = WIFI_INTERFACE_STATION;
  uint32_t ipv4 = 0;

  sdep(SDEP_CMD_GET_GATEWAY_ADDRESS, 1, &interface, NULL, &ipv4);
  return ipv4;
}

/******************************************************************************/
/*!
    @brief Get current connected AP's SSID
*/
/******************************************************************************/
char* AdafruitFeather::SSID (void)
{
  if ( !_connected ) return NULL;
	return _ap_info.ssid;
}

/******************************************************************************/
/*!
    @brief Get current connected AP's BSSID
*/
/******************************************************************************/
uint8_t* AdafruitFeather::BSSID (uint8_t* bssid)
{
  if ( !_connected ) return NULL;
  memcpy(bssid, _ap_info.bssid, 6);
  return bssid;
}

/******************************************************************************/
/*!
    @brief Get current connected AP's encryption type
*/
/******************************************************************************/
int32_t AdafruitFeather::encryptionType (void)
{
  if ( !_connected ) return 0;
  return _ap_info.security;
}

/******************************************************************************/
/*!
    @brief Get RSSI of the currently connected AP
    @return RSSI value or 0 if not connected
*/
/******************************************************************************/
int32_t AdafruitFeather::RSSI (void)
{
  if ( !_connected ) return 0;

  int32_t rssi;
  sdep(SDEP_CMD_WIFI_GET_RSSI, 0, NULL, NULL, &rssi);

  return rssi;
}

bool AdafruitFeather::saveConnectedProfile(void)
{
  if ( !_connected ) return false;

  return sdep(SDEP_CMD_WIFI_PROFILE_SAVE, 0, NULL, NULL, NULL);
}

bool AdafruitFeather::addProfile(char* ssid)
{
  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid), .p_value = ssid      },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_WIFI_PROFILE_ADD, para_count, para_arr, NULL, NULL);
}

bool AdafruitFeather::addProfile(char* ssid, char* key, int enc_type)
{
  if ( enc_type == ENC_TYPE_AUTO )
  {
    Serial.println("Encryption cannot be AUTO for now");
    return false;
  }

  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid), .p_value = ssid      },
      { .len = strlen(key) , .p_value = key       },
      { .len = 4           , .p_value = &enc_type },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_WIFI_PROFILE_ADD, para_count, para_arr, NULL, NULL);
}

bool AdafruitFeather::removeProfile(char* ssid)
{
  return sdep(SDEP_CMD_WIFI_PROFILE_DEL, strlen(ssid), ssid, NULL, NULL);
}

void AdafruitFeather::clearProfiles(void)
{
  sdep(SDEP_CMD_WIFI_PROFILE_CLEAR, 0, NULL, NULL, NULL);
}

bool AdafruitFeather::checkProfile(char* ssid)
{
  bool result = false;
  sdep(SDEP_CMD_WIFI_PROFILE_CHECK, strlen(ssid), ssid, NULL, &result);
  return result;
}

char* AdafruitFeather::profileSSID (uint8_t pos)
{
  static char profile_ssid[32+1];

  uint8_t option = 1; // requesting SSID

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 1, .p_value = &pos    },
      { .len = 1, .p_value = &option },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  uint16_t len = 0;

  sdep_n(SDEP_CMD_WIFI_PROFILE_GET, para_count, para_arr, &len, profile_ssid);

  return (len > 0) ? profile_ssid : NULL;
}

int32_t AdafruitFeather::profileEncryptionType(uint8_t pos)
{
  uint8_t option = 2; // requesting Encryption Type

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 1, .p_value = &pos    },
      { .len = 1, .p_value = &option },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  int32_t sec_type;

  return sdep_n(SDEP_CMD_WIFI_PROFILE_GET, para_count, para_arr, NULL, &sec_type) ? sec_type : -1;
}

/******************************************************************************/
/*!
    @brief  Performs DNS lookup of a specified hostname

    @param[in]      hostname        hostname to lookup

    @return Resolved IPAdress, if failed INADDR_NONE(0.0.0.) is returned
*/
/******************************************************************************/
IPAddress AdafruitFeather::hostByName( const char* hostname)
{
  IPAddress result = INADDR_NONE;
  this->hostByName(hostname, result);
  return result;
}

/******************************************************************************/
/*!
    @brief  Performs DNS lookup of a specified hostname

    @param[in]      hostname        hostname to lookup
    @param[in]      result          Resovled IP

    @return true if succeeded, false otherwise.
*/
/******************************************************************************/
bool AdafruitFeather::hostByName( const char* hostname, IPAddress& result)
{
  VERIFY(_connected);

  uint32_t ip_addr = 0;
  VERIFY( sdep(SDEP_CMD_DNSLOOKUP, strlen(hostname), hostname, NULL, &ip_addr) );

  result = ip_addr;
  return true;
}

/******************************************************************************/
/*!
    @brief  Return the results from an ICMP ping against a host

    @param[in]  host      Can be a hostname (e.g "adafruit.com") or IP address
                          in string formet (e.g "8.8.8.8")

    @return Response time in milliseconds, 0 if failed
*/
/******************************************************************************/
uint32_t AdafruitFeather::ping(char const* host)
{
  // Resolve IP
  IPAddress ipaddr = this->hostByName(host);
  if (ipaddr == INADDR_NONE) return 0;

  return this->ping(ipaddr);
}

/******************************************************************************/
/*!
    @brief  Return the results from an ICMP ping against a specified IP address

    @param[in]  ipaddr   IP address

    @return Response time in milliseconds, 0 if failed
*/
/******************************************************************************/
uint32_t AdafruitFeather::ping(IPAddress ipaddr)
{
  uint32_t response_time;
  uint32_t ipv4 = (uint32_t) ipaddr;

  return sdep(SDEP_CMD_PING, 4, &ipv4, NULL, &response_time) ? response_time : 0;
}

/******************************************************************************/
/*!
    @brief  Starts scanning for WiFi APs in range

    @param[out] ap_list  List of scanned AP
    @param[out] max_ap   Number of AP that the buffer can hold

    @return     Number of scanned AP
*/
/******************************************************************************/
int AdafruitFeather::scanNetworks(wl_ap_info_t ap_list[], uint8_t max_ap)
{
  uint16_t length = max_ap*sizeof(wl_ap_info_t);
  VERIFY_RETURN( sdep(SDEP_CMD_SCAN, 0, NULL, &length, ap_list), 0);

  return length/sizeof(wl_ap_info_t);
}

/******************************************************************************/
/*!
    @brief  Disconnects from the access point if a connection exists

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
void AdafruitFeather::disconnect(void)
{
  _connected = !sdep(SDEP_CMD_DISCONNECT, 0, NULL, NULL, NULL);
}

#if 0
/******************************************************************************/
/*!
    @brief  Set Root CA certificates in PEM format. PEM format is base64 encoded,
            and in form

            -----BEGIN CERTIFICATE-----
            ..........KEYS.............
            -----END CERTIFICATE-----

    @note   Feather works natively with DER (binary) format, therefore it will convert
    PEM --> DER format and need to malloc 2-3KB of SRAM hold the result. To save
    Memory usage, please consider to use the PEM format.
*/
/******************************************************************************/
bool AdafruitFeather::setRootCertificatesPEM(char const* root_certs_pem)
{
  uint16_t len = (root_certs_pem ? strlen(root_certs_pem) : 0 );
//  return sdep(SDEP_CMD_TLS_SET_ROOT_CERTS, len, root_certs_pem, NULL, NULL);
  // TODO use sdep instead of sdep_n
  sdep_cmd_para_t para_arr[] =
  {
      { .len = len  , .p_value = root_certs_pem },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_TLS_SET_ROOT_CERTS, para_count, para_arr, NULL, NULL);

}
#endif

/******************************************************************************/
/*!
    @brief  Set Root CA certificates in DER format. DER format is binary format,
            and is the native format that Feather works with.

    @note   Feather works natively with DER (binary) format, therefore it will work
    on the DER format as it is. This typically saves 2-3KB of SRAM comparing to the
    PEM counterpart.
*/
/******************************************************************************/
bool AdafruitFeather::setRootCertificates(uint8_t const* root_certs_der, uint16_t len)
{
//  return sdep(SDEP_CMD_TLS_SET_ROOT_CERTS, len, root_certs_der, NULL, NULL);
  // TODO use sdep instead of sdep_n
  sdep_cmd_para_t para_arr[] =
  {
      { .len = len  , .p_value = root_certs_der },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_TLS_SET_ROOT_CERTS, para_count, para_arr, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Starts AP mode on the module, causing the module to advertise a new
            Wireless network and SSID, etc.

    @param[in]      ssid    SSID of Wi-Fi access point

    @param[in]      passwd  Password of Wi-Fi access point

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ssid and passwd are combined in a single payload buffer which are
            separated by ',' character

            The passwd could be NULL or empty string if it does not exist
*/
/******************************************************************************/
err_t AdafruitFeather::startAP(char* ssid, char* passwd)
{
  if (ssid == NULL || ssid == "" || passwd == NULL) return ERROR_SDEP_INVALIDPARAMETER;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(ssid)  , .p_value = ssid   },
      { .len = strlen(passwd), .p_value = passwd },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  err_t err = FEATHERLIB->sdep_execute_n(SDEP_CMD_APSTART, para_count, para_arr, NULL, NULL);
  return err;
}

/******************************************************************************/
/*!
    @brief  Starts AP mode on the module with default SSID and password

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
err_t AdafruitFeather::startAP(void)
{
  return FEATHERLIB->sdep_execute(SDEP_CMD_APSTART, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Stop AP mode on the module

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
err_t AdafruitFeather::stopAP(void)
{
  return FEATHERLIB->sdep_execute(SDEP_CMD_APSTOP, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Get the current UTC date and time using the ISO 8601 standard
            e.g. "2012-07-02T17:12:34.567890Z" (string length = 27 bytes)

    @param[out]     iso8601_time    String of current UTC date and time
*/
/******************************************************************************/
bool AdafruitFeather::getISO8601Time(iso8601_time_t* iso8601_time)
{
  iso8601_time->nullterm = 0;
  return sdep(SDEP_CMD_GET_ISO8601_TIME, 0, NULL, NULL, iso8601_time);
}

/******************************************************************************/
/*!
    @brief  Get the current UTC in seconds

    @return UTC time if success, 0 otherwise
*/
/******************************************************************************/
uint32_t AdafruitFeather::getUtcTime(void)
{
  uint32_t utc_time;
  VERIFY_RETURN(sdep(SDEP_CMD_GET_UTC_TIME, 0, NULL, NULL, &utc_time), 0);

  return utc_time;
}

/******************************************************************************/
/*!
    @brief  Send HTTP request (GET or POST) to server

    @param[in]    url            URL (e.g. www.adafruit.com/testwifi/testpost.php
                                      or   www.adafruit.com/testwifi/index.html)

    @param[in]    content        Request content (e.g. var1=value1&var2=value2)

    @param[in]    method         HTTP method (GET_METHOD or POST_METHOD)

    @param[in]    buffer_length  Length of the output buffer provided by user

    @param[out]   buffer         Output buffer

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
err_t AdafruitFeather::httpRequest(const char* url, const char* content, uint8_t method,
                                        uint32_t buffer_length, uint8_t* buffer)
{
  if (url == NULL || url == "") return ERROR_SDEP_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_SDEP_INVALIDPARAMETER;

  uint16_t paylen = strlen(url) + sizeof(buffer_length) + 3; // 2 null terminators & 1 byte for METHOD
  if (content != NULL) paylen += strlen(content);

  uint8_t* payload = (uint8_t*)malloc(paylen);
  uint8_t* p_payload = payload;

  memcpy(p_payload, (uint8_t*)url, strlen(url));
  p_payload += strlen(url);
  *p_payload++ = 0;
  if (content != NULL)
  {
    memcpy(p_payload, (uint8_t*)content, strlen(content));
    p_payload += strlen(content);
  }
  *p_payload++ = 0;
  *p_payload++ = method;
  memcpy(p_payload, (uint8_t*)&buffer_length, sizeof(buffer_length));

  err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_HTTPREQUEST, paylen,
                                               payload, NULL, buffer);
  free(payload);
  return error;
}

/******************************************************************************/
/*!
    @brief  Send HTTPS request (GET or POST) to server

    @param[in]    url            URL (e.g. www.adafruit.com/testwifi/testpost.php
                                      or   www.adafruit.com/testwifi/index.html)


    @param[in]    ca_cert        Address of certificate chain

    @param[in]    content        Request content (e.g. var1=value1&var2=value2)

    @param[in]    method         HTTP method (GET_METHOD or POST_METHOD)

    @param[in]    buffer_length  Length of the output buffer provided by user

    @param[out]   buffer         Output buffer

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
err_t AdafruitFeather::httpsRequest(const char* url, const char* ca_cert,
                                         const char* content, uint8_t method,
                                         uint32_t buffer_length, uint8_t* buffer)
{
  if (url == NULL || url == "") return ERROR_SDEP_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_SDEP_INVALIDPARAMETER;

  uint32_t cert_addr = (uint32_t)ca_cert;

  // 2 null terminators & 1 byte for METHOD are added to payload buffer
  uint16_t paylen = strlen(url) + sizeof(cert_addr) + sizeof(buffer_length) + 3;
  if (content != NULL) paylen += strlen(content);

  uint8_t* payload = (uint8_t*)malloc(paylen);
  uint8_t* p_payload = payload;

  /* URL */
  memcpy(p_payload, (uint8_t*)url, strlen(url));
  p_payload += strlen(url);
  *p_payload++ = 0;

  /* Certificate memory address */
  memcpy(p_payload, (uint8_t*)&cert_addr, sizeof(cert_addr));
  p_payload += sizeof(cert_addr);

  /* Request content */
  if (content != NULL)
  {
    memcpy(p_payload, (uint8_t*)content, strlen(content));
    p_payload += strlen(content);
  }
  *p_payload++ = 0;

  /* Method */
  *p_payload++ = method;

  /* Length of output buffer */
  memcpy(p_payload, (uint8_t*)&buffer_length, sizeof(buffer_length));

  err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_HTTPSREQUEST, paylen,
                                               payload, NULL, buffer);
  free(payload);
  return error;
}

/******************************************************************************/
/*!
    @brief  Send HTTP request (GET or POST) to server

    @param[in]    url            URL (e.g. www.adafruit.com/testwifi/testpost.php
                                      or   www.adafruit.com/testwifi/index.html)

    @param[in]    content        Request content (e.g. var1=value1&var2=value2)

    @param[in]    method         HTTP method (GET_METHOD or POST_METHOD)

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
err_t AdafruitFeather::httpRequestWithCallback(const char* url, const char* content, uint8_t method)
{
  if (url == NULL || strlen(url) == 0) return ERROR_SDEP_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_SDEP_INVALIDPARAMETER;

  uint16_t paylen = strlen(url) + 3; // 2 null terminators & 1 byte for METHOD
  if ( (content != NULL) && (strlen(content) > 0) )
  {
    paylen += strlen(content);
  }

  uint8_t* payload = (uint8_t*)malloc(paylen);
  uint8_t* p_payload = payload;

  /* URL */
  memcpy(p_payload, (uint8_t*)url, strlen(url));
  p_payload += strlen(url);
  *p_payload++ = 0;

  /* Content */
  if ( (content != NULL) && (strlen(content) > 0) )
  {
    memcpy(p_payload, (uint8_t*)content, strlen(content));
    p_payload += strlen(content);
  }
  *p_payload++ = 0;

  /* Method */
  *p_payload = method;

  err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_HTTPREQUESTWITHCB, paylen,
                                               payload, NULL, NULL);
  free(payload);
  return error;
}

/******************************************************************************/
/*!
    @brief  Send HTTPS request (GET or POST) to server with certificate chain

    @param[in]    url            URL (e.g. www.adafruit.com/testwifi/testpost.php
                                      or   www.adafruit.com/testwifi/index.html)

    @param[in]    ca_cert        Address of certificate chain

    @param[in]    content        Request content (e.g. var1=value1&var2=value2)

    @param[in]    method         HTTP method (GET_METHOD or POST_METHOD)

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
err_t AdafruitFeather::httpsRequestWithCallback(const char* url, const char* ca_cert,
                                                     const char* content, uint8_t method)
{
  if (url == NULL || strlen(url) == 0) return ERROR_SDEP_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_SDEP_INVALIDPARAMETER;

  uint32_t cert_addr = 0;
  if (ca_cert != NULL) cert_addr = (uint32_t)ca_cert;

  uint16_t paylen = strlen(url) + sizeof(cert_addr) + 3; // 2 null terminators & 1 byte for METHOD
  if ( (content != NULL) && (strlen(content) > 0) )
  {
    paylen += strlen(content);
  }

  uint8_t* payload = (uint8_t*)malloc(paylen);
  uint8_t* p_payload = payload;

  /* URL */
  memcpy(p_payload, (uint8_t*)url, strlen(url));
  p_payload += strlen(url);
  *p_payload++ = 0;

  /* Certificate memory address */
  memcpy(p_payload, (uint8_t*)&cert_addr, sizeof(cert_addr));
  p_payload += sizeof(cert_addr);

  /* Content */
  if ( (content != NULL) && (strlen(content) > 0) )
  {
    memcpy(p_payload, (uint8_t*)content, strlen(content));
    p_payload += strlen(content);
  }
  *p_payload++ = 0;

  /* Method */
  *p_payload = method;

  err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_HTTPSREQUESTWITHCB, paylen,
                                               payload, NULL, NULL);
  free(payload);
  return error;
}

/******************************************************************************/
/*!
    @brief  Register the async callback handler for HTTP

    @param[in]    ada_httpCallback    HTTP Callback handler from Arduino code
*/
/******************************************************************************/
void AdafruitFeather::addHttpDataReceivedCallBack(ada_http_callback ada_httpCallback)
{
  ada_http_rx_callback = ada_httpCallback;
}

//--------------------------------------------------------------------+
// Callbacks
//--------------------------------------------------------------------+
/******************************************************************************/
/*!
    @brief This callback is invoked when WIFI_INTERFACE_STATION is up
*/
/******************************************************************************/
//void adafruit_wifi_connect_callback(void)
//{
//  feather._connected = true;
//  if ( feather.wlan_connect_callback )
//  {
//    feather.wlan_connect_callback();
//  }
//}

/******************************************************************************/
/*!
    @brief This callback is invoked when WIFI_INTERFACE_STATION is down
*/
/******************************************************************************/
void adafruit_wifi_disconnect_callback(void)
{
  Feather._connected = false;
  if ( Feather.wlan_disconnect_callback )
  {
    Feather.wlan_disconnect_callback();
  }
}

/******************************************************************************/
/*!
    @brief  HTTP Callback from featherlib when new data received
*/
/******************************************************************************/
void http_rx_callback(uint8_t* data, uint16_t data_length, uint16_t avail)
{
  if (Feather.ada_http_rx_callback != NULL)
  {
    Feather.ada_http_rx_callback(data, data_length, avail);
  }
}
