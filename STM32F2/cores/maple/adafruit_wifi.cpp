/**************************************************************************/
/*!
    @file     adafruit_wifi.cpp
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

#include "adafruit_wicedlib.h"
#include "adafruit_wifi.h"
#include <string.h>

/******************************************************************************/
/*!
    @brief Instantiates a new instance of the AdafruitWICED class
*/
/******************************************************************************/
AdafruitWICED::AdafruitWICED(void)
{
//  init();
}

/******************************************************************************/
/*!
    @brief Initialization if necessary
*/
/******************************************************************************/
void AdafruitWICED::init()
{

}

/******************************************************************************/
/*!
    @brief  Starts scanning for WiFi APs in range

    @param[out]     ap_details  A buffer storing scanned AP information

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::scan(uint16_t* length, uint8_t* ap_details)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_SCAN, 0, NULL, length, ap_details);
}

/******************************************************************************/
/*!
    @brief  Attempt to connect to the specified access point

    @param[in]      ssid    SSID of Wi-Fi access point

    @param[in]      passwd  Password of Wi-Fi access point

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ssid and passwd are combined in a single payload buffer which are
            separated by ',' character
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::connectAP(char* ssid, char* passwd)
{
  uint8_t ssid_len = strlen(ssid);
  uint8_t pass_len = strlen(passwd);
  char payload[ssid_len + pass_len + 2];

  memcpy(&payload[0], ssid, ssid_len);
  payload[ssid_len] = ',';
  memcpy(&payload[ssid_len + 1], passwd, pass_len);
  payload[ssid_len + pass_len + 1] = 0;

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_CONNECT, strlen(payload),
                                       (uint8_t*)payload, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Attempt to connect to the specified access point with no password

    @param[in]      ssid    SSID of Wi-Fi access point

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::connectAP(char* ssid)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_CONNECT, strlen(ssid),
                                       (uint8_t*)ssid, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Disconnects from the access point if a connection exists

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::disconnectAP(void)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_DISCONNECT, 0, NULL, NULL, NULL);
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
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::startAP(char* ssid, char* passwd)
{
  uint8_t ssid_len = strlen(ssid);
  uint8_t pass_len = strlen(passwd);
  char payload[ssid_len + pass_len + 2];

  memcpy(&payload[0], ssid, ssid_len);
  payload[ssid_len] = ',';
  memcpy(&payload[ssid_len + 1], passwd, pass_len);
  payload[ssid_len + pass_len + 1] = 0;

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_APSTART, strlen(payload),
                                       (uint8_t*)payload, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Starts AP mode on the module with default SSID and password

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::startAP(void)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_APSTART, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Stop AP mode on the module

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::stopAP(void)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_APSTOP, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Return the results from an ICMP ping against a specified IP address

    @param[in]      ip_address_str    String of ip address (e.g. "192.168.1.100")

    @param[out]     response_time     The ping response time

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::ping(char* ip_address_str, uint8_t* response_time)
{
  uint8_t payload[4] = {0};
  char* ptr_ip = ip_address_str;

  // Parse IP address string and check if the IP address is valid
  // The valid IP address is stored in payload buffer
  uint8_t index = 0;
  while (*ptr_ip)
  {
    if (*ptr_ip >= '0' && *ptr_ip <= '9')
    {
      if (index > 3) return ERROR_INVALIDPARAMETER;
      payload[index] *= 10;
      payload[index] += *ptr_ip - '0';
    }
    else if (*ptr_ip == '.')
    {
      index++;
    }
    else return ERROR_INVALIDPARAMETER;
    ptr_ip++;
  }

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_PING, 4, payload,
                                       NULL, response_time);
}

/******************************************************************************/
/*!
    @brief  Return the results from an ICMP ping against a specified IP address

    @param[in]      ip_address        An array of 4 bytes IP address

    @param[out]     response_time     The ping response time

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::ping(uint8_t* ip_address, uint8_t* response_time)
{
  // Check if IP address is valid
  for (int i = 0; i < 3; i++)
  {
    if (ip_address[i] > 255) return ERROR_INVALIDPARAMETER;
  }

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_PING, 4, ip_address,
                                       NULL, response_time);
}

/******************************************************************************/
/*!
    @brief  Performs DNS lookup of a specified domain name for its IPv4 IP address

    @param[in]      dns             The domain name to lookup

    @param[out]     ipv4_address    IPv4 IP address that is associated with
                                    the requested domain name

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::dnsLookup(char* dns, uint8_t* ipv4_address)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_DNSLOOKUP, strlen(dns),
                                       (uint8_t*)dns, NULL, ipv4_address);
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
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::getTime()
{

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
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::httpGetUri(char* uri, uint8_t* response)
{

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
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::httpPost(char* uri, uint8_t* response)
{

}

AdafruitWICED wiced;
