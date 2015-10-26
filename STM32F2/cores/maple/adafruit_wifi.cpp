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

#include "adafruit_featherlib.h"
#include "adafruit_wifi.h"
#include "itoa.h"
#include <string.h>
#include <stdlib.h>

/******************************************************************************/
/*!
    @brief Instantiates a new instance of the AdafruitFeather class
*/
/******************************************************************************/
AdafruitFeather::AdafruitFeather(void)
{
//  init();
}

/******************************************************************************/
/*!
    @brief Initialization if necessary
*/
/******************************************************************************/
void AdafruitFeather::init()
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
sdep_err_t AdafruitFeather::scan(uint16_t* length, uint8_t* ap_details)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_SCAN, 0, NULL, length, ap_details);
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

            The passwd could be NULL or empty string if it does not exist
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::connectAP(char* ssid, char* passwd)
{
  if (ssid == NULL || ssid == "") return ERROR_INVALIDPARAMETER;

  uint16_t payload_len = strlen(ssid);
  if (passwd != NULL) payload_len += strlen(passwd) + 1;

  char* payload = (char*)malloc(payload_len);

  strcpy(payload, ssid);
  if (passwd != NULL)
  {
    strcat(payload, ",");
    strcat(payload, passwd);
  }

  sdep_err_t error = ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_CONNECT, payload_len,
                                                       (uint8_t*)payload, NULL, NULL);
  free(payload);
  return error;
}

/******************************************************************************/
/*!
    @brief  Disconnects from the access point if a connection exists

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::disconnectAP(void)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_DISCONNECT, 0, NULL, NULL, NULL);
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
sdep_err_t AdafruitFeather::startAP(char* ssid, char* passwd)
{
  if (ssid == NULL || ssid == "") return ERROR_INVALIDPARAMETER;

  uint16_t payload_len = strlen(ssid);
  if (passwd != NULL) payload_len += strlen(passwd) + 1;

  char* payload = (char*)malloc(payload_len);

  strcpy(payload, ssid);
  if (passwd != NULL)
  {
    strcat(payload, ",");
    strcat(payload, passwd);
  }

  sdep_err_t error = ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_APSTART, payload_len,
                                                       (uint8_t*)payload, NULL, NULL);
  free(payload);
  return error;
}

/******************************************************************************/
/*!
    @brief  Starts AP mode on the module with default SSID and password

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::startAP(void)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_APSTART, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Stop AP mode on the module

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::stopAP(void)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_APSTOP, 0, NULL, NULL, NULL);
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
sdep_err_t AdafruitFeather::ping(char* ip_address_str, uint8_t* response_time)
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

  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_PING, 4, payload,
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
sdep_err_t AdafruitFeather::ping(uint8_t* ip_address, uint8_t* response_time)
{
  // Check if IP address is valid
  for (int i = 0; i < 3; i++)
  {
    if (ip_address[i] > 255) return ERROR_INVALIDPARAMETER;
  }

  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_PING, 4, ip_address,
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
sdep_err_t AdafruitFeather::dnsLookup(char* dns, uint8_t* ipv4_address)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_DNSLOOKUP, strlen(dns),
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
sdep_err_t AdafruitFeather::getTime(char* iso8601_time)
{

  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_GETTIME, 0, NULL, NULL,
                                           (uint8_t*)iso8601_time);
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
sdep_err_t AdafruitFeather::httpGetUri(char* uri, uint16_t* length, uint8_t* response)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_HTTPGETURI, strlen(uri),
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
sdep_err_t AdafruitFeather::httpPost(char* uri, uint16_t* length, uint8_t* response)
{
  if (uri == NULL) return ERROR_INVALIDPARAMETER;

  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_HTTPPOST, strlen(uri),
                                           (uint8_t*)uri, length, response);
}

/******************************************************************************/
/*!
    @brief  Specify the LastWill message used in case of ungraceful disconnection

    @param[in]    topic    Topic

    @param[in]    value    Value

    @param[in]    qos      Quality of Service

    @param[in]    retain   Retain

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            LastWill message format: <topic>,<value>,<qos[1]>,<retain[0?]>
            e.g. "topic/adafruit,offline,1,0"
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::mqttLastWill(bool isOnlineTopic, char* topic, char* value, uint8_t qos, uint8_t retain)
{
  if (topic == NULL) return ERROR_INVALIDPARAMETER;

  uint16_t lastWillMessage_len = strlen(topic) + 6; // isOnlineTopic, qos, retain & 3 commas
  if (value != NULL) lastWillMessage_len += strlen(value);
  char* lastWillMessage = (char*)malloc(lastWillMessage_len);

  if (isOnlineTopic)
  {
    strcpy(lastWillMessage, "1");
  }
  else
  {
    strcpy(lastWillMessage, "0");
  }
  strcat(lastWillMessage, topic);
  strcat(lastWillMessage, ",");
  if (value != NULL) strcat(lastWillMessage, value);
  strcat(lastWillMessage, ",");
  char str[2];
  utoa(qos, str, 10);
  strcat(lastWillMessage, str);
  strcat(lastWillMessage, ",");
  utoa(retain, str, 10);
  strcat(lastWillMessage, str);

  sdep_err_t error = ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_MQTTLASTWILL, lastWillMessage_len,
                                                       (uint8_t*)lastWillMessage, NULL, NULL);
  free(lastWillMessage);
  return error;
}

/******************************************************************************/
/*!
    @brief        Connect to a broker specified by host name, port, client ID,
                  user name and password

    @param[in]    host       Host name

    @param[in]    port       Port (Value from 0 to 65535)

    @param[in]    clientID   Client ID

    @param[in]    username   User name

    @param[in]    password   Password

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            MQTT Server format: <host>,<port>,<client ID>,<user name>,<password>
            e.g. "85.119.83.194,1883,adafruit,mosfet,IOkey" or
                 "test.mosquitto.org,1883,adafruit,mosfet,IOkey"

            If string parameters do not exist, passing NULL or empty string as
            function's parameters. Then the default parameters are used.

            Value of zero for port parameter means port is not specified. Then
            the default port (1883) is used.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::mqttConnect(char* host, uint16_t port, char* clientID,
                                        char* username, char* password, bool is_tls)
{
  if (host == NULL || host == "") return ERROR_INVALIDPARAMETER;

  char p_port[6];
  utoa(port, p_port, 10);

  uint16_t mqttServer_len = strlen(host) + 5; // isSSLConnection & 4 commas
  if (port > 0) mqttServer_len += strlen(p_port);
  if (clientID != NULL) mqttServer_len += strlen(clientID);
  if (username != NULL) mqttServer_len += strlen(username);
  if (password != NULL) mqttServer_len += strlen(password);

  char* mqttBroker = (char*)malloc(mqttServer_len);
  if (mqttBroker == NULL) return ERROR_NO_MEMORY;

  if (is_tls)
  {
    strcpy(mqttBroker, "1");
  }
  else
  {
    strcpy(mqttBroker, "0");
  }
  strcat(mqttBroker, host);
  strcat(mqttBroker, ",");
  if (port > 0) strcat(mqttBroker, p_port);
  strcat(mqttBroker, ",");
  if (clientID != NULL) strcat(mqttBroker, clientID);
  strcat(mqttBroker, ",");
  if (username != NULL) strcat(mqttBroker, username);
  strcat(mqttBroker, ",");
  if (password != NULL) strcat(mqttBroker, password);

  sdep_err_t error =  ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_MQTTCONNECT, mqttServer_len,
                                                        (uint8_t*)mqttBroker, NULL, NULL);
  free(mqttBroker);
  return error;
}

/******************************************************************************/
/*!
    @brief  Disconnect from a broker

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::mqttDisconnect(void)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_MQTTDISCONNECT, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Publish a message to a specified topic

    @param[in]    topic    Topic

    @param[in]    value    Value

    @param[in]    qos      Quality of Service

    @param[in]    retain   Retain

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            Published message format: <topic>,<value>,<qos[1]>,<retain[0?]>
            e.g. "topic/adafruit,hello,1,0"
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::mqttPublish(char* topic, char* value, uint8_t qos, uint8_t retain)
{
  if (topic == NULL) return ERROR_INVALIDPARAMETER;

  uint16_t publishedMessage_len = strlen(topic) + 5; // qos, retain & 3 commas
  if (value != NULL) publishedMessage_len += strlen(value);
  char* publishedMessage = (char*)malloc(publishedMessage_len);

  strcpy(publishedMessage, topic);
  strcat(publishedMessage, ",");
  if (value != NULL) strcat(publishedMessage, value);
  strcat(publishedMessage, ",");
  char str[2];
  utoa(qos, str, 10);
  strcat(publishedMessage, str);
  strcat(publishedMessage, ",");
  utoa(retain, str, 10);
  strcat(publishedMessage, str);

  sdep_err_t error = ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_MQTTPUBLISH, publishedMessage_len,
                                                       (uint8_t*)publishedMessage, NULL, NULL);
  free(publishedMessage);
  return error;
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
sdep_err_t AdafruitFeather::mqttSubscribe(char* topic, uint8_t qos)
{
  if (topic == NULL) return ERROR_INVALIDPARAMETER;

  uint16_t subTopic_len = strlen(topic) + 2; // qos & a comma
  char* subTopic = (char*)malloc(subTopic_len);

  strcpy(subTopic, topic);
  strcat(subTopic, ",");
  char str[2];
  utoa(qos, str, 10);
  strcat(subTopic, str);

  sdep_err_t error = ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_MQTTSUBSCRIBE, subTopic_len,
                                                       (uint8_t*)subTopic, NULL, NULL);
  free(subTopic);
  return error;
}

/******************************************************************************/
/*!
    @brief  Un-subscribe from a subscribed topic

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::mqttUnsubscribe(void)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_MQTTUNSUBSCRIBE, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Read from the ASYNC FIFO

    @param[out]   response_length  Length of the first arrived message in the FIFO

    @param[out]   response         The first arrived message in the FIFO

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::irqRead(uint16_t* response_length, uint8_t* response)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_IRQREAD, 0, NULL,
                                           response_length, response);
}

/******************************************************************************/
/*!
    @brief  Return the current number of items in the ASYNC FIFO

    @param[out]   n_items     Number of items

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::irqCount(uint16_t* n_items)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_IRQCOUNT, 0, NULL, NULL,
                                           (uint8_t*)n_items);
}

/******************************************************************************/
/*!
    @brief  Return the number of records LEFT in the ASYNC FIFO

    @param[out]  n_available  Number of records LEFT

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::irqAvailable(uint16_t* n_available)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_IRQAVAIL, 0, NULL, NULL,
                                           (uint8_t*)n_available);
}

/******************************************************************************/
/*!
    @brief  Clear the ASYNC FIFO and deassert IRQ

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::irqClear(void)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_IRQCLEAR, 0, NULL, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Clear the ASYNC FIFO and deassert IRQ

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::httpsGet(char* host, const char* root_ca_cert, const char* query,
                                     uint32_t buffer_length, uint8_t* buffer)
{
  return ADAFRUIT_FEATHERLIB->feather_sdep(SDEP_CMD_HTTPSGET, 0, NULL, NULL, NULL);
}


AdafruitFeather feather;
