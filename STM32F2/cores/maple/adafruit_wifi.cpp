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
#include "itoa.h"
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

    @param[out]     length      Length of ap_details buffer

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
    @brief  Get the current UTC date and time using the ISO 8601 standard
            e.g. "2012-07-02T17:12:34.567890Z" (string length = 27 bytes)

    @param[out]     iso8601_time    String of current UTC date and time

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::getTime(char* iso8601_time)
{

  sdep_err_t error = ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_GETTIME, 0, NULL,
                                                   NULL, (uint8_t*)iso8601_time);
  return error;
}

/******************************************************************************/
/*!
    @brief  Retrieve the results of a specified URI

    @param[in]      uri       The URI

    @param[out]     length    Length of response buffer

    @param[out]     response  The full response based on the specified URI

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::httpGetUri(char* uri, uint16_t* length, uint8_t* response)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_HTTPGETURI, strlen(uri),
                                       (uint8_t*)uri, length, response);
}

/******************************************************************************/
/*!
    @brief  Send the 'x-www-form-urlencoded' URL as a HTTP POST operation

    @param[in]      uri       The URI (must contain parameters for the post
                                       after a '?' character in the URI)

    @param[out]     length    Length of response buffer

    @param[out]     response  The full response based on the specified URI

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::httpPost(char* uri, uint16_t* length, uint8_t* response)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_HTTPPOST, strlen(uri),
                                       (uint8_t*)uri, length, response);
}

/******************************************************************************/
/*!
    @brief  Specify the LastWill message used in case of ungraceful disconnection

    @param[in]    lastWillMessage   String of LastWill message including topic,
                                    value, qos and retain

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            LastWill message format: <topic>,<value>,<qos[1]>,<retain[0?]>
            e.g. "topic/adafruit,offline,1,0"
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::mqttLastWill(char* topic, char* value, uint8_t qos, uint8_t retain)
{
  uint16_t lastWillMessage_len = strlen(topic) + strlen(value) + 5; // qos, retain & 3 commas
  char lastWillMessage[lastWillMessage_len];
  char* p_lastWillMessage = lastWillMessage;

  strcpy(p_lastWillMessage, topic);
  strcat(p_lastWillMessage, ",");
  strcat(p_lastWillMessage, value);
  strcat(p_lastWillMessage, ",");
  char str[2];
  utoa(qos, str, 10);
  strcat(p_lastWillMessage, str);
  strcat(p_lastWillMessage, ",");
  utoa(retain, str, 10);
  strcat(p_lastWillMessage, str);

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_MQTTLASTWILL, lastWillMessage_len,
                                       (uint8_t*)lastWillMessage, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Connect to a broker specified by host name, port and client ID

    @param[in]    mqttServer      String of mqttServer including host name, port,
                                  and client ID

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            MQTT Server format: <host>,<port>,<client ID>
            e.g. "85.119.83.194,1883,adafruit" or
                 "test.mosquitto.org,1883,adafruit"
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::mqttConnect(char* host, uint16_t port, char* clientID)
{
  char p_port[4];
  utoa(port, p_port, 10);

  uint16_t mqttServer_len = strlen(host) + strlen(p_port) + strlen(clientID) + 2;
  char mqttServer[mqttServer_len];
  char* p_mqttServer = mqttServer;

  strcpy(p_mqttServer, host);
  strcat(p_mqttServer, ",");
  strcat(p_mqttServer, p_port);
  strcat(p_mqttServer, ",");
  strcat(p_mqttServer, clientID);

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_MQTTCONNECT, mqttServer_len,
                                       (uint8_t*)p_mqttServer, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Disconnect from a broker

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::mqttDisconnect(void)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_MQTTDISCONNECT, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Publish a message to a specified topic

    @param[in]    lastWillMessage   String of published message including topic,
                                    value, qos and retain

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            Published message format: <topic>,<value>,<qos[1]>,<retain[0?]>
            e.g. "topic/adafruit,hello,1,0"
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::mqttPublish(char* topic, char* value, uint8_t qos, uint8_t retain)
{
  uint16_t publishedMessage_len = strlen(topic) + strlen(value) + 5; // qos, retain & 3 commas
  char publishedMessage[publishedMessage_len];
  char* p_publishedMessage = publishedMessage;

  strcpy(p_publishedMessage, topic);
  strcat(p_publishedMessage, ",");
  strcat(p_publishedMessage, value);
  strcat(p_publishedMessage, ",");
  char str[2];
  utoa(qos, str, 10);
  strcat(p_publishedMessage, str);
  strcat(p_publishedMessage, ",");
  utoa(retain, str, 10);
  strcat(p_publishedMessage, str);

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_MQTTPUBLISH, publishedMessage_len,
                                       (uint8_t*)publishedMessage, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Subscribe to a specified topic

    @param[in]    topic   String of topic to subscribe including topic, and qos

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            Topic format: <topic>,<qos>
            e.g. "topic/adafruit,0"

            Incoming messages will be handled using ASYNC FIFO
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::mqttSubscribe(char* topic, uint8_t qos)
{
  uint16_t subTopic_len = strlen(topic) + 2; // qos & a comma
  char subTopic[subTopic_len];
  char* p_subTopic = subTopic;

  strcpy(p_subTopic, topic);
  strcat(p_subTopic, ",");
  char str[2];
  utoa(qos, str, 10);
  strcat(p_subTopic, str);

  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_MQTTSUBSCRIBE, subTopic_len,
                                       (uint8_t*)subTopic, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Un-subscribe from a subscribed topic

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitWICED::mqttUnsubscribe(void)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_MQTTUNSUBSCRIBE, 0, NULL, NULL, NULL);
}


sdep_err_t AdafruitWICED::irqRead(uint16_t* response_length, uint8_t* response)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_IRQREAD, 0, NULL, response_length, response);
}
sdep_err_t AdafruitWICED::irqCount(uint16_t* n_items)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_IRQCOUNT, 0, NULL, NULL, (uint8_t*)n_items);
}
sdep_err_t AdafruitWICED::irqAvailable(uint16_t* n_available)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_IRQAVAIL, 0, NULL, NULL, (uint8_t*)n_available);
}
sdep_err_t AdafruitWICED::irqClear(void)
{
  return ADAFRUIT_WICEDLIB->wiced_sdep(SDEP_CMD_IRQCLEAR, 0, NULL, NULL, NULL);
}

AdafruitWICED wiced;
