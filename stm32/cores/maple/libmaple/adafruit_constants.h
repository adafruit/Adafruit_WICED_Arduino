/**************************************************************************/
/*!
    @file     adafruit_constants.h
    @author   hathach

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

#ifndef _ADAFRUIT_CONSTANTS_H_
#define _ADAFRUIT_CONSTANTS_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define SHARED_ENABLED     0x00008000
#define WPA_SECURITY       0x00200000
#define WPA2_SECURITY      0x00400000
#define ENTERPRISE_ENABLED 0x02000000
#define WPS_ENABLED        0x10000000
#define IBSS_ENABLED       0x20000000

#define OPEN_AUTH                 0x0000
#define SHARED_AUTH               0x0001
#define WEP_ENABLED               0x0001
#define TKIP_ENABLED              0x0002
#define AES_ENABLED               0x0004
#define WSEC_SWFLAG               0x0008

enum
{
  WIFI_INTERFACE_STATION = 0,
  WIFI_INTERFACE_AP      = 1,
  WIFI_INTERFACE_P2P     = 2,
};

typedef enum
{
  ENC_TYPE_AUTO       = -1,                                               /**< Auto detection                         */
  ENC_TYPE_OPEN       = 0,                                                /**< Open security                           */

  // WEP
  ENC_TYPE_WEP        = WEP_ENABLED,                                      /**< WEP Security with open authentication   */
  ENC_TYPE_WEP_SHARED = ( WEP_ENABLED | SHARED_ENABLED ),                 /**< WEP Security with shared authentication */

  // WPA Personal
  ENC_TYPE_WPA_TKIP   = ( WPA_SECURITY  | TKIP_ENABLED ),                 /**< WPA Security with TKIP                  */
  ENC_TYPE_WPA_AES    = ( WPA_SECURITY  | AES_ENABLED ),                  /**< WPA Security with AES                   */
  ENC_TYPE_WPA_MIXED  = ( WPA_SECURITY  | AES_ENABLED | TKIP_ENABLED ),   /**< WPA Security with AES & TKIP            */

  // WPA2 Personal
  ENC_TYPE_WPA2_AES   = ( WPA2_SECURITY | AES_ENABLED ),                  /**< WPA2 Security with AES                  */
  ENC_TYPE_WPA2_TKIP  = ( WPA2_SECURITY | TKIP_ENABLED ),                 /**< WPA2 Security with TKIP                 */
  ENC_TYPE_WPA2_MIXED = ( WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED ),   /**< WPA2 Security with AES & TKIP           */

  // WPA Enterprise
  ENC_TYPE_WPA_TKIP_ENT   = ( ENTERPRISE_ENABLED | WPA_SECURITY  | TKIP_ENABLED ),
  ENC_TYPE_WPA_AES_ENT    = ( ENTERPRISE_ENABLED | WPA_SECURITY  | AES_ENABLED ),
  ENC_TYPE_WPA_MIXED_ENT  = ( ENTERPRISE_ENABLED | WPA_SECURITY  | AES_ENABLED | TKIP_ENABLED ),

  // WPA2 Enterprise
  ENC_TYPE_WPA2_TKIP_ENT  = ( ENTERPRISE_ENABLED | WPA2_SECURITY | TKIP_ENABLED ),
  ENC_TYPE_WPA2_AES_ENT   = ( ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED ),
  ENC_TYPE_WPA2_MIXED_ENT = ( ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED ),

  // WPS
  ENC_TYPE_WPS_OPEN   = ( WPS_ENABLED ),                                   /**< WPS with open security                  */
  ENC_TYPE_WPS_SECURE = ( WPS_ENABLED | AES_ENABLED),                      /**< WPS with AES security                   */

  // IBSS
  ENC_TYPE_IBSS_OPEN      = ( IBSS_ENABLED )
} wl_enc_type_t;


/**
 * @enum err_t
 * @brief Error opcodes
 */
typedef enum
{
  ERROR_NONE                       = 0,
  ERROR_OUT_OF_HEAP_SPACE          = 8,
  ERROR_NOT_CONNECTED              = 20,

  ERROR_WWD_INVALID_KEY            = 1004,  /**< WWD 1004, Invalid AP Password */
  ERROR_WWD_AUTHENTICATION_FAILED  = 1006,  /**< WWD 1006, Authentication error */
  ERROR_WWD_NETWORK_NOT_FOUND      = 1024,  /**< WWD 1024, likely a range issue */
  ERROR_WWD_UNABLE_TO_JOIN         = 1025,  /**< WWD 1025, likely a range issue */
  ERROR_WWD_ACCESS_POINT_NOT_FOUND = 1066,

  ERROR_SDEP_INVALIDPARAMETER      = 30002,
} err_t;

/**
 * SDEP Command ID
 */
typedef enum {
  SDEP_CMD_RESET               = 0x0001, /**< HW reset                             */          //!< SDEP_CMD_RESET
  SDEP_CMD_FACTORYRESET        = 0x0002, /**< Factory reset (reset NVM & erase Arduino code) *///!< SDEP_CMD_FACTORYRESET
  SDEP_CMD_DFU                 = 0x0003, /**< Go into DFU mode                     */          //!< SDEP_CMD_DFU
  SDEP_CMD_INFO                = 0x0004, /**< System information                   */          //!< SDEP_CMD_INFO
  SDEP_CMD_NVM_RESET           = 0x0005, /**< Reset NVM/DCT setting                */          //!< SDEP_CMD_NVM_RESET
  SDEP_CMD_ERROR_STRING        = 0x0006, /**< Get descriptive error string         */

  /* GPIO Command */
  SDEP_CMD_GPIO                = 0x0010, /**< Set GPIO                             */          //!< SDEP_CMD_GPIO
  SDEP_CMD_RANDOMNUMBER        = 0x0011, /**< Random Number                        */          //!< SDEP_CMD_RANDOMNUMBER

  /* WiFi Commands */
  SDEP_CMD_SCAN                = 0x0020, /**< AP scan                              */          //!< SDEP_CMD_SCAN
  SDEP_CMD_CONNECT             = 0x0021, /**< Connect to AP                        */          //!< SDEP_CMD_CONNECT
  SDEP_CMD_DISCONNECT          = 0x0022, /**< Disconnect from AP                   */          //!< SDEP_CMD_DISCONNECT
  SDEP_CMD_APSTART             = 0x0023, /**< Start AP                             */          //!< SDEP_CMD_APSTART
  SDEP_CMD_APSTOP              = 0x0024, /**< Stop AP                              */          //!< SDEP_CMD_APSTOP
  SDEP_CMD_WIFI_GET_RSSI       = 0x0025, ///< Get current RSSI
  SDEP_CMD_WIFI_PROFILE_ADD    = 0x0026, ///< Add a network profile
  SDEP_CMD_WIFI_PROFILE_DEL    = 0x0027, ///< Add a network profile
  SDEP_CMD_WIFI_PROFILE_CLEAR  = 0x0028, ///< Clear all network profiles
  SDEP_CMD_WIFI_PROFILE_CHECK  = 0x0029, ///< Check if a network profile existed
  SDEP_CMD_WIFI_PROFILE_SAVE   = 0x002A, ///< Save the current connect AP to profile list
  SDEP_CMD_WIFI_PROFILE_GET    = 0x002B, ///< Get AP's profile info
  SDEP_CMD_TLS_SET_ROOT_CERTS  = 0x002C, ///< Set ROOT CA Chain
  SDEP_CMD_TLS_REQUIRE_VERIFICATION = 0x002D, ///< Enable/Disable TLS server's verification

  /* Network Commands */
  SDEP_CMD_PING                = 0x0030, /**< Ping                                 */          //!< SDEP_CMD_PING
  SDEP_CMD_DNSLOOKUP           = 0x0031, /**< DNS lookup                           */          //!< SDEP_CMD_DNSLOOKUP
  SDEP_CMD_GET_ISO8601_TIME    = 0x0032, /**< Get time in ISO8601 format           */          //!< SDEP_CMD_GET_ISO8601_TIME
  SDEP_CMD_HTTPGETURI          = 0x0033, /**< HTTP Get URI                         */          //!< SDEP_CMD_HTTPGETURI
  SDEP_CMD_HTTPPOST            = 0x0034, /**< HTTP Post                            */          //!< SDEP_CMD_HTTPPOST
  SDEP_CMD_HTTPREQUEST         = 0x0035, /**< HTTP Request                         */          //!< SDEP_CMD_HTTPREQUEST
  SDEP_CMD_HTTPSREQUEST        = 0x0036, /**< HTTPs Request                        */          //!< SDEP_CMD_HTTPSREQUEST
  SDEP_CMD_HTTPREQUESTWITHCB   = 0x0037, /**< Async HTTP Request                   */          //!< SDEP_CMD_HTTPREQUESTWITHCB
  SDEP_CMD_HTTPSREQUESTWITHCB  = 0x0038, /**< Async HTTPS Request                  */          //!< SDEP_CMD_HTTPSREQUESTWITHCB
  SDEP_CMD_HTTPSENDREQUEST     = 0x0039, /**< Send HTTP Request (HTTP Class)       */          //!< SDEP_CMD_HTTPSENDREQUEST
  SDEP_CMD_HTTPDATALENGTH      = 0x003A, /**< Get the length of packet's data      */          //!< SDEP_CMD_HTTPDATALENGTH
  SDEP_CMD_HTTPRESPONSECODE    = 0x003B, /**< Get response code                    */          //!< SDEP_CMD_HTTPRESPONSECODE
  SDEP_CMD_HTTPEXTRACTHEADER   = 0x003C, /**< Extract HTTP header                  */          //!< SDEP_CMD_HTTPEXTRACTHEADER
  SDEP_CMD_HTTPENDOFHEADER     = 0x003D, /**< Check if end of HTTP header reached  */          //!< SDEP_CMD_HTTPENDOFHEADER
  SDEP_CMD_HTTPSKIPHEADER      = 0x003E, /**< Skip HTTP header                     */          //!< SDEP_CMD_HTTPSKIPHEADER
  SDEP_CMD_GET_UTC_TIME        = 0x003F, ///< Get UTC time in seconds

  /* DEBUG Commands */
  SDEP_CMD_STACKDUMP           = 0x0040, /**< Dump the stack                       */          //!< SDEP_CMD_STACKDUMP
  SDEP_CMD_STACKSIZE           = 0x0041, /**< Get stack size                       */          //!< SDEP_CMD_STACKSIZE
  SDEP_CMD_HEAPDUMP            = 0x0042, /**< Dump the heap                        */          //!< SDEP_CMD_HEAPDUMP
  SDEP_CMD_HEAPSIZE            = 0x0043, /**< Get heap size                        */          //!< SDEP_CMD_HEAPSIZE
  SDEP_CMD_THREADLIST          = 0x0044, /**< Get Thread information               */          //!< SDEP_CMD_THREADLIST

  /* SPI Flash Commands */
  SDEP_CMD_SFLASHFORMAT        = 0x0050, /**< Format SPI flash memory              */          //!< SDEP_CMD_SFLASHFORMAT
  SDEP_CMD_SFLASHLIST          = 0x0051, /**< List SPI flash contents              */          //!< SDEP_CMD_SFLASHLIST

  /* MQTT Commands */
  SDEP_CMD_MQTTLASTWILL        = 0x0060, /**< Get Last Will message                */          //!< SDEP_CMD_MQTTLASTWILL
  SDEP_CMD_MQTTCONNECT         = 0x0061, /**< Connect to a broker                  */          //!< SDEP_CMD_MQTTCONNECT
  SDEP_CMD_MQTTDISCONNECT      = 0x0062, /**< Disconnect from a broker             */          //!< SDEP_CMD_MQTTDISCONNECT
  SDEP_CMD_MQTTPUBLISH         = 0x0063, /**< Publish a message to a topic         */          //!< SDEP_CMD_MQTTPUBLISH
  SDEP_CMD_MQTTSUBSCRIBE       = 0x0064, /**< Subscribe to a topic                 */          //!< SDEP_CMD_MQTTSUBSCRIBE
  SDEP_CMD_MQTTUNSUBSCRIBE     = 0x0065, /**< Unsubscribe from a topic             */          //!< SDEP_CMD_MQTTUNSUBSCRIBE

  /* IRQ Commands */
  SDEP_CMD_IRQREAD             = 0x0070, /**< Read from async response fifo        */          //!< SDEP_CMD_IRQREAD
  SDEP_CMD_IRQCOUNT            = 0x0071, /**< Number of records in async fifo      */          //!< SDEP_CMD_IRQCOUNT
  SDEP_CMD_IRQAVAIL            = 0x0072, /**< Number of records LEFT in async fifo */          //!< SDEP_CMD_IRQAVAIL
  SDEP_CMD_IRQCLEAR            = 0x0073, /**< Clear async fifo & deassert IRQ      */          //!< SDEP_CMD_IRQCLEAR

  /* Gateway Commands */
  SDEP_CMD_GET_IPV4_ADDRESS    = 0x0080, /**< Get IPv4 Address                     */          //!< SDEP_CMD_GET_IPV4_ADDRESS
  SDEP_CMD_GET_IPV6_ADDRESS    = 0x0081, /**< Get IPv6 Address                     */          //!< SDEP_CMD_GET_IPV6_ADDRESS
  SDEP_CMD_GET_GATEWAY_ADDRESS = 0x0082, /**< Get Gateway's IPv4 Address           */          //!< SDEP_CMD_GET_GATEWAY_ADDRESS
  SDEP_CMD_GET_NETMASK         = 0x0083, /**< Get Netmask's IPv4 Address           */          //!< SDEP_CMD_GET_NETMASK
  SDEP_CMD_GET_MAC_ADDRESS     = 0x0084, /**< Get MAC Address                      */          //!< SDEP_CMD_GET_MAC_ADDRESS

  /* TCP Commnads */
  SDEP_CMD_TCP_CONNECT         = 0x0090,                                                       //!< SDEP_CMD_TCP_CONNECT
  SDEP_CMD_TCP_WRITE           = 0x0091,                                                       //!< SDEP_CMD_TCP_WRITE
  SDEP_CMD_TCP_FLUSH           = 0x0092,                                                       //!< SDEP_CMD_TCP_FLUSH
  SDEP_CMD_TCP_READ            = 0x0093,                                                       //!< SDEP_CMD_TCP_READ
  SDEP_CMD_TCP_CLOSE           = 0x0094,                                                       //!< SDEP_CMD_TCP_CLOSE
  SDEP_CMD_TCP_AVAILABLE       = 0x0095,                                                       //!< SDEP_CMD_TCP_AVAILABLE
  SDEP_CMD_TCP_PEEK            = 0x0096,                                                       //!< SDEP_CMD_TCP_PEEK
  SDEP_CMD_TCP_STATUS          = 0x0097,                                                       //!< SDEP_CMD_TCP_STATUS
  SDEP_CMD_TCP_SET_CALLBACK    = 0x0098,                                                       //!< SDEP_CMD_TCP_SET_CALLBACK

  /* UDP Commands */
  SDEP_CMD_UDP_CREATE          = 0x00A0,                                                       //!< SDEP_CMD_UDP_CREATE
  SDEP_CMD_UDP_WRITE           = 0x00A1,                                                       //!< SDEP_CMD_UDP_WRITE
  SDEP_CMD_UDP_FLUSH           = 0x00A2,                                                       //!< SDEP_CMD_UDP_FLUSH
  SDEP_CMD_UDP_READ            = 0x00A3,                                                       //!< SDEP_CMD_UDP_READ
  SDEP_CMD_UDP_CLOSE           = 0x00A4,                                                       //!< SDEP_CMD_UDP_CLOSE
  SDEP_CMD_UDP_AVAILABLE       = 0x00A5,                                                       //!< SDEP_CMD_UDP_AVAILABLE
  SDEP_CMD_UDP_PEEK            = 0x00A6,                                                       //!< SDEP_CMD_UDP_PEEK
  SDEP_CMD_UDP_PACKET_INFO     = 0x00A7,                                                       //!< SDEP_CMD_UDP_PACKET_INFO
} sdep_command_t;

/**
 * @enum mqtt_evt_opcode_t
 * @brief MQTT event opcodes
 */
typedef enum
{
  MQTT_EVT_INVALID          = 0x00,  /**< Invalid event code            */
  MQTT_EVT_DISCONNECTED     = 0x81,  /**< Link disconnected event code  */
  MQTT_EVT_TOPIC_CHANGED    = 0x82   /**< Topic changed event code      */

} mqtt_evt_opcode_t;

enum
{
    TLS_NO_VERIFICATION       = 0,
    TLS_VERIFICATION_OPTIONAL = 1,
    TLS_VERIFICATION_REQUIRED = 2,
};

#ifdef __cplusplus
 }
#endif

#endif /* _ADAFRUIT_CONSTANTS_H_ */
