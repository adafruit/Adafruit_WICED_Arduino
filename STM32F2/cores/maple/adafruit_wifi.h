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
#include <string.h>

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
#define ERROR_NONE               0x0000
#define ERROR_INVALIDPARAMETER   0x8025
#define ERROR_NO_MEMORY          0x80AA   /**< Temporary */

typedef enum {
  SDEP_CMD_RESET           = 0x0001, /**< HW reset                             */
  SDEP_CMD_FACTORYRESET    = 0x0002, /**< Factory reset                        */
  SDEP_CMD_DFU             = 0x0003, /**< Go into DFU mode                     */
  SDEP_CMD_INFO            = 0x0004, /**< System information                   */
  /* GPIO Command */
  SDEP_CMD_GPIO            = 0x0010, /**< Set GPIO                             */
  SDEP_CMD_RANDOMNUMBER    = 0x0011, /**< Random Number                        */
  /* WiFi Commands */
  SDEP_CMD_SCAN            = 0x0020, /**< AP scan                              */
  SDEP_CMD_CONNECT         = 0x0021, /**< Connect to AP                        */
  SDEP_CMD_DISCONNECT      = 0x0022, /**< Disconnect from AP                   */
  SDEP_CMD_APSTART         = 0x0023, /**< Start AP                             */
  SDEP_CMD_APSTOP          = 0x0024, /**< Stop AP                              */
  /* Network Commands */
  SDEP_CMD_PING            = 0x0030, /**< Ping                                 */
  SDEP_CMD_DNSLOOKUP       = 0x0031, /**< DNS lookup                           */
  SDEP_CMD_GETTIME         = 0x0032, /**< Get time                             */
  SDEP_CMD_HTTPGETURI      = 0x0033, /**< HTTP Get URI                         */
  SDEP_CMD_HTTPPOST        = 0x0034, /**< HTTP Post                            */
  SDEP_CMD_HTTPSGET        = 0x0035, /**< HTTPS Get                            */
  SDEP_CMD_HTTPREQUEST     = 0x0036, /**< HTTP Request                         */
  SDEP_CMD_HTTPSREQUEST     = 0x0037, /**< HTTPs Request                        */
  SDEP_CMD_ASYNCHTTPREQUEST = 0x0038, /**< Async HTTP Request                   */
  /* DEBUG Commands */
  SDEP_CMD_STACKDUMP       = 0x0040, /**< Dump the stack                       */
  SDEP_CMD_STACKSIZE       = 0x0041, /**< Get stack size                       */
  SDEP_CMD_HEAPDUMP        = 0x0042, /**< Dump the heap                        */
  SDEP_CMD_HEAPSIZE        = 0x0043, /**< Get heap size                        */
  SDEP_CMD_THREADLIST      = 0x0044, /**< Get Thread information               */
  /* SPI Flash Commands */
  SDEP_CMD_SFLASHFORMAT    = 0x0050, /**< Format SPI flash memory              */
  SDEP_CMD_SFLASHLIST      = 0x0051, /**< List SPI flash contents              */
  /* MQTT Commands */
  SDEP_CMD_MQTTLASTWILL    = 0x0060, /**< Get Last Will message                */
  SDEP_CMD_MQTTCONNECT     = 0x0061, /**< Connect to a broker                  */
  SDEP_CMD_MQTTDISCONNECT  = 0x0062, /**< Disconnect from a broker             */
  SDEP_CMD_MQTTPUBLISH     = 0x0063, /**< Publish a message to a topic         */
  SDEP_CMD_MQTTSUBSCRIBE   = 0x0064, /**< Subscribe to a topic                 */
  SDEP_CMD_MQTTUNSUBSCRIBE = 0x0065, /**< Unsubscribe from a topic             */
  /* IRQ Commands */
  SDEP_CMD_IRQREAD         = 0x0070, /**< Read from async response fifo        */
  SDEP_CMD_IRQCOUNT        = 0x0071, /**< Number of records in async fifo      */
  SDEP_CMD_IRQAVAIL        = 0x0072, /**< Number of records LEFT in async fifo */
  SDEP_CMD_IRQCLEAR        = 0x0073, /**< Clear async fifo & deassert IRQ      */
} sdep_command_t;

enum {
  GET_METHOD    = 0x01,
  POST_METHOD   = 0x02,
};

typedef uint16_t sdep_err_t;

extern "C"
{
  void http_callback(uint8_t* data, uint16_t data_length, uint16_t available);
  /* Callback prototypes */
  typedef void (*ada_http_rx_callback)(uint8_t* data, uint16_t data_length, uint16_t available);
}

class AdafruitFeather
{
private:
  void init();

  ada_http_rx_callback  ada_http_callback;

public:
  AdafruitFeather(void);

  sdep_err_t randomNumber(uint32_t* random32bit);

  /* WiFi Commands */
  sdep_err_t scan(uint16_t* length, uint8_t* ap_details);
  sdep_err_t connectAP(char* ssid, char* passwd);
  sdep_err_t disconnectAP(void);
  sdep_err_t startAP(char* ssid, char* passwd);
  sdep_err_t startAP(void);
  sdep_err_t stopAP(void);

  /* Network Commands */
  sdep_err_t ping(char* ip_address_str, uint8_t* response_time);
  sdep_err_t ping(uint8_t* ip_address, uint8_t* response_time);
  sdep_err_t dnsLookup(char* dns, uint8_t* ipv4_address);
  sdep_err_t getTime(char* iso8601_time);
  sdep_err_t httpGetUri(char* uri, uint16_t* length, uint8_t* response);
  sdep_err_t httpPost(char* uri, uint16_t* length, uint8_t* response);
  sdep_err_t httpsGet(char* host, const char* root_ca_cert, const char* query, uint32_t buffer_length, uint8_t* buffer);

  sdep_err_t httpRequest(const char* url, const char* content, uint8_t method, uint32_t buffer_length, uint8_t* buffer);
  sdep_err_t httpsRequest(const char* url, const char* root_ca_cert, const char* content, uint8_t method, uint32_t buffer_length, uint8_t* buffer);
  sdep_err_t asyncHttpRequest(const char* url, const char* content, uint8_t method);
  void       addHttpDataReceivedCallBack(ada_http_rx_callback ada_httpCallback = NULL);

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
  sdep_err_t mqttConnect(char* host, uint16_t port, char* clientID, char* username, char* password, bool is_tls);
  sdep_err_t mqttDisconnect(void);
  sdep_err_t mqttPublish(char* topic, char* value, uint8_t qos, uint8_t retain);
  sdep_err_t mqttSubscribe(char* topic, uint8_t qos);
  sdep_err_t mqttUnsubscribe(void);

  /* IRQ Commands */
  sdep_err_t irqRead(uint16_t* response_length, uint8_t* response);
  sdep_err_t irqCount(uint16_t* n_items);
  sdep_err_t irqAvailable(uint16_t* n_available);
  sdep_err_t irqClear(void);

  /* callback from featherlib */
  friend void http_callback(uint8_t* data, uint16_t data_length, uint16_t available);
};

extern AdafruitFeather feather;

#endif
