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


AdafruitFeather feather;

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
    @brief  Return a 32-bit random number

    @param[out]     random32bit       The returned 32-bit random number

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::randomNumber(uint32_t* random32bit)
{
  return FEATHERLIB->sdep_execute(SDEP_CMD_RANDOMNUMBER, 0, NULL, NULL, (uint8_t*)random32bit);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_SCAN, 0, NULL, length, ap_details);
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
sdep_err_t AdafruitFeather::connectAP(char const* ssid, char const* passwd)
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

  sdep_err_t error = FEATHERLIB->sdep_execute(SDEP_CMD_CONNECT, payload_len,
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_DISCONNECT, 0, NULL, NULL, NULL);
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

  sdep_err_t error = FEATHERLIB->sdep_execute(SDEP_CMD_APSTART, payload_len,
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_APSTART, 0, NULL, NULL, NULL);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_APSTOP, 0, NULL, NULL, NULL);
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

  return FEATHERLIB->sdep_execute(SDEP_CMD_PING, 4, payload, NULL, response_time);
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

  return FEATHERLIB->sdep_execute(SDEP_CMD_PING, 4, ip_address, NULL, response_time);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_DNSLOOKUP, strlen(dns),
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

  return FEATHERLIB->sdep_execute(SDEP_CMD_GETTIME, 0, NULL, NULL, (uint8_t*)iso8601_time);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_HTTPGETURI, strlen(uri),
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

  return FEATHERLIB->sdep_execute(SDEP_CMD_HTTPPOST, strlen(uri),
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

  sdep_err_t error = FEATHERLIB->sdep_execute(SDEP_CMD_MQTTLASTWILL, lastWillMessage_len,
                                              (uint8_t*)lastWillMessage, NULL, NULL);
  free(lastWillMessage);
  return error;
}

/**************************************************************************/
/*!
    @brief  Return a random MQTT Client ID

    @param  clientID[out]       Random client ID

    @param  length[in]          Number of characters

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/**************************************************************************/
sdep_err_t AdafruitFeather::mqttGenerateRandomID(char* clientID, uint8_t length)
{
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  uint8_t i;
  sdep_err_t error;
  uint32_t random32bit;
  for (i = 0; i < length; i++)
  {
    /* Get the random number */
    if ( (error = randomNumber(&random32bit)) == 0)
    {
      clientID[i] = alphanum[random32bit % (sizeof(alphanum) -  1)];
    }
    else return error;
  }
  clientID[length] = 0;
}

/******************************************************************************/
/*!
    @brief        Connect to a broker specified by host name, port, client ID,
                  user name and password (TLS disabled)

    @param[in]    host       Host name

    @param[in]    port       Port (Value from 0 to 65535)

    @param[in]    clientID   Client ID

    @param[in]    username   User name

    @param[in]    password   Password

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            MQTT Server format: <host>,<port>,<client ID>,<user name>,<password>
            e.g. "85.119.83.194,1883,adafruit,user,IOkey" or
                 "test.mosquitto.org,1883,adafruit,user,IOkey"

            If string parameters do not exist, passing NULL or empty string as
            function's parameters. Then the default parameters are used.

            Value of zero for port parameter means port is not specified. Then
            the default port (1883) is used.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::mqttConnect(char* host, uint16_t port, char* clientID,
                                        char* username, char* password)
{
  if (host == NULL || host == "") return ERROR_INVALIDPARAMETER;

  char p_port[6];
  utoa(port, p_port, 10);

  uint16_t mqttServer_len = strlen(host) + 9;      // isSSLConnection, 4 commas
                                                   // & 4 bytes certificate address
  if (port > 0) mqttServer_len += strlen(p_port);
  if (clientID != NULL) mqttServer_len += strlen(clientID);
  if (username != NULL) mqttServer_len += strlen(username);
  if (password != NULL) mqttServer_len += strlen(password);

  uint8_t* payload = (uint8_t*)malloc(mqttServer_len);
  if (payload == NULL) return ERROR_NO_MEMORY;

  uint8_t* p_payload = payload;
  p_payload[0] = 0;

  /* Certificate memory address */
  uint32_t cert_addr = 0;
  memcpy(&p_payload[1], (uint8_t*)&cert_addr, sizeof(cert_addr));
  p_payload += sizeof(cert_addr) + 1;

  memcpy(p_payload, (uint8_t*)host, strlen(host));
  p_payload += strlen(host);
  *p_payload++ = ',';

  if (port > 0)
  {
    memcpy(p_payload, (uint8_t*)p_port, strlen(p_port));
    p_payload += strlen(p_port);
  }
  *p_payload++ = ',';

  memcpy(p_payload, (uint8_t*)clientID, strlen(clientID));
  p_payload += strlen(clientID);
  *p_payload++ = ',';

  memcpy(p_payload, (uint8_t*)username, strlen(username));
  p_payload += strlen(username);
  *p_payload++ = ',';

  memcpy(p_payload, (uint8_t*)password, strlen(password));

  sdep_err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_MQTTCONNECT, mqttServer_len,
                                               payload, NULL, NULL);
  free(payload);
  return error;
}

/******************************************************************************/
/*!
    @brief        Connect to a broker specified by host name, port, client ID,
                  user name and password with TLS connection

    @param[in]    host       Host name

    @param[in]    port       Port (Value from 0 to 65535)

    @param[in]    clientID   Client ID

    @param[in]    username   User name

    @param[in]    password   Password

    @param[in]    ca_cert    Address of CA certificate chain (NULL means unspecified)

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            MQTT Server format: <host>,<port>,<client ID>,<user name>,<password>
            e.g. "85.119.83.194,8883,adafruit,user,IOkey" or
                 "test.mosquitto.org,8883,adafruit,user,IOkey"

            If string parameters do not exist, passing NULL or empty string as
            function's parameters. Then the default parameters are used.

            Value of zero for port parameter means port is not specified. Then
            the default port (8883) is used.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::mqttTLSConnect(char* host, uint16_t port, char* clientID,
                                           char* username, char* password, const char* ca_cert)
{
  if (host == NULL || host == "") return ERROR_INVALIDPARAMETER;

  char p_port[6];
  utoa(port, p_port, 10);

  uint16_t mqttServer_len = strlen(host) + 9;      // isSSLConnection, 4 commas
                                                   // & 4 bytes certificate address
  if (port > 0) mqttServer_len += strlen(p_port);
  if (clientID != NULL) mqttServer_len += strlen(clientID);
  if (username != NULL) mqttServer_len += strlen(username);
  if (password != NULL) mqttServer_len += strlen(password);

  uint8_t* payload = (uint8_t*)malloc(mqttServer_len);
  if (payload == NULL) return ERROR_NO_MEMORY;

  uint8_t* p_payload = payload;

  /* Enable TLS connection */
  p_payload[0] = 1;

  /* Certificate memory address */
  uint32_t cert_addr = 0;
  if (ca_cert != NULL) cert_addr = (uint32_t)ca_cert;
  memcpy(&p_payload[1], (uint8_t*)&cert_addr, sizeof(cert_addr));
  p_payload += sizeof(cert_addr) + 1;

  memcpy(p_payload, (uint8_t*)host, strlen(host));
  p_payload += strlen(host);
  *p_payload++ = ',';

  if (port > 0)
  {
    memcpy(p_payload, (uint8_t*)p_port, strlen(p_port));
    p_payload += strlen(p_port);
  }
  *p_payload++ = ',';

  memcpy(p_payload, (uint8_t*)clientID, strlen(clientID));
  p_payload += strlen(clientID);
  *p_payload++ = ',';

  memcpy(p_payload, (uint8_t*)username, strlen(username));
  p_payload += strlen(username);
  *p_payload++ = ',';

  memcpy(p_payload, (uint8_t*)password, strlen(password));

  sdep_err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_MQTTCONNECT, mqttServer_len,
                                               payload, NULL, NULL);
  free(payload);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_MQTTDISCONNECT, 0, NULL, NULL, NULL);
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

  sdep_err_t error = FEATHERLIB->sdep_execute(SDEP_CMD_MQTTPUBLISH, publishedMessage_len,
                                              (uint8_t*)publishedMessage, NULL, NULL);
  free(publishedMessage);
  return error;
}

/******************************************************************************/
/*!
    @brief  Subscribe to a specified topic

    @param[in]    topic    Subscribed topic

    @param[in]    qos      Quality of Service

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.

    @note   ',' characters are used to separate parameters together
            Topic format: <topic>,<qos>
            e.g. "topic/adafruit,0"

            Incoming messages will be handled using ASYNC FIFO & callback function
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

  sdep_err_t error = FEATHERLIB->sdep_execute(SDEP_CMD_MQTTSUBSCRIBE, subTopic_len,
                                              (uint8_t*)subTopic, NULL, NULL);
  free(subTopic);
  return error;
}

/******************************************************************************/
/*!
    @brief  Un-subscribe from a subscribed topic

    @param[in]    topic    Un-subscribed topic

    @return Returns ERROR_NONE (0x0000) if everything executed properly, otherwise
            a specific error if something went wrong.
*/
/******************************************************************************/
sdep_err_t AdafruitFeather::mqttUnsubscribe(char* topic)
{
  if (topic == NULL || topic[0] == 0) return ERROR_INVALIDPARAMETER;

  return FEATHERLIB->sdep_execute(SDEP_CMD_MQTTUNSUBSCRIBE, strlen(topic),
                                  (uint8_t*)topic, NULL, NULL);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_IRQREAD, 0, NULL, response_length, response);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_IRQCOUNT, 0, NULL, NULL, (uint8_t*)n_items);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_IRQAVAIL, 0, NULL, NULL, (uint8_t*)n_available);
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
  return FEATHERLIB->sdep_execute(SDEP_CMD_IRQCLEAR, 0, NULL, NULL, NULL);
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
sdep_err_t AdafruitFeather::httpRequest(const char* url, const char* content, uint8_t method,
                                        uint32_t buffer_length, uint8_t* buffer)
{
  if (url == NULL || url == "") return ERROR_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_INVALIDPARAMETER;

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

  sdep_err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_HTTPREQUEST, paylen,
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
sdep_err_t AdafruitFeather::httpsRequest(const char* url, const char* ca_cert,
                                         const char* content, uint8_t method,
                                         uint32_t buffer_length, uint8_t* buffer)
{
  if (url == NULL || url == "") return ERROR_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_INVALIDPARAMETER;

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

  sdep_err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_HTTPSREQUEST, paylen,
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
sdep_err_t AdafruitFeather::asyncHttpRequest(const char* url, const char* content, uint8_t method)
{
  if (url == NULL || strlen(url) == 0) return ERROR_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_INVALIDPARAMETER;

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

  sdep_err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_ASYNCHTTPREQUEST, paylen,
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
sdep_err_t AdafruitFeather::asyncHttpsRequest(const char* url, const char* ca_cert,
                                              const char* content, uint8_t method)
{
  if (url == NULL || strlen(url) == 0) return ERROR_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_INVALIDPARAMETER;

  uint32_t cert_addr = (uint32_t)ca_cert;

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

  sdep_err_t error =  FEATHERLIB->sdep_execute(SDEP_CMD_ASYNCHTTPSREQUEST, paylen,
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

/******************************************************************************/
/*!
    @brief  Register the MQTT event callback handler

    @param[in]    ada_mqttCallback    MQTT Callback handler from Arduino code
*/
/******************************************************************************/
void AdafruitFeather::addMqttCallBack(ada_mqtt_callback ada_mqttCallback)
{
  ada_mqtt_evt_callback = ada_mqttCallback;
}

/******************************************************************************/
/*!
    @brief  HTTP Callback from featherlib when new data received
*/
/******************************************************************************/
void http_rx_callback(uint8_t* data, uint16_t data_length, uint16_t avail)
{
  if (feather.ada_http_rx_callback != NULL)
  {
    feather.ada_http_rx_callback(data, data_length, avail);
  }
}

/******************************************************************************/
/*!
    @brief  MQTT Callback from featherlib when a new event occurs
*/
/******************************************************************************/
void mqtt_evt_callback(mqtt_evt_opcode_t event, uint16_t len, uint8_t* data)
{
  if (feather.ada_mqtt_evt_callback != NULL)
  {
    feather.ada_mqtt_evt_callback(event, len, data);
  }
}
