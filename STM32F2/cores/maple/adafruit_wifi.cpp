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

    @param[in/out]  ap_details  A buffer storing scanned AP information

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::scan(uint8_t* ap_details)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_SCAN, 0, NULL, ap_details);
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
  char payload[ssid_len + pass_len + 1];

  memcpy(&payload[0], ssid, ssid_len);
  payload[ssid_len] = ',';
  memcpy(&payload[ssid_len + 1], passwd, pass_len);

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_CONNECT, ssid_len + pass_len + 1,
                                       (uint8_t*)payload, NULL);
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
                                       (uint8_t*)ssid, NULL);
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
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_DISCONNECT, 0, NULL, NULL);
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
  char payload[ssid_len + pass_len + 1];

  memcpy(&payload[0], ssid, ssid_len);
  payload[ssid_len] = ',';
  memcpy(&payload[ssid_len + 1], passwd, pass_len);

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_APSTART, ssid_len + pass_len + 1,
                                       (uint8_t*)payload, NULL);
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
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_APSTART, 0, NULL, NULL);
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
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_APSTOP, 0, NULL, NULL);
}

AdafruitWICED wiced;
