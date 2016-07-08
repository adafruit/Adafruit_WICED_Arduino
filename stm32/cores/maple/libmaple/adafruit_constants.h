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

enum
{
  WIFI_INTERFACE_STATION = 0,
  WIFI_INTERFACE_AP      = 1,
  WIFI_INTERFACE_P2P     = 2,
};

#define ENC_SHARED_ENABLED            0x00008000
#define ENC_WPA_SECURITY              0x00200000
#define ENC_WPA2_SECURITY             0x00400000
#define ENC_ENTERPRISE_ENABLED        0x02000000
#define ENC_WPS_ENABLED               0x10000000
#define ENC_IBSS_ENABLED              0x20000000

#define ENC_OPEN_AUTH                 0x0000
#define ENC_SHARED_AUTH               0x0001
#define ENC_WEP_ENABLED               0x0001
#define ENC_TKIP_ENABLED              0x0002
#define ENC_AES_ENABLED               0x0004
#define ENC_WSEC_SWFLAG               0x0008

typedef enum
{
  ENC_TYPE_AUTO           = -1,                                                         /**< Auto detection                         */
  ENC_TYPE_OPEN           = 0,                                                          /**< Open security                           */

  // WEP
  ENC_TYPE_WEP            = ENC_WEP_ENABLED,                                            /**< WEP Security with open authentication   */
  ENC_TYPE_WEP_SHARED     = ( ENC_WEP_ENABLED | ENC_SHARED_ENABLED ),                   /**< WEP Security with shared authentication */

  // WPA Personal
  ENC_TYPE_WPA_TKIP       = ( ENC_WPA_SECURITY  | ENC_TKIP_ENABLED ),                   /**< WPA Security with TKIP                  */
  ENC_TYPE_WPA_AES        = ( ENC_WPA_SECURITY  | ENC_AES_ENABLED ),                    /**< WPA Security with AES                   */
  ENC_TYPE_WPA_MIXED      = ( ENC_WPA_SECURITY  | ENC_AES_ENABLED | ENC_TKIP_ENABLED ), /**< WPA Security with AES & TKIP            */

  // WPA2 Personal
  ENC_TYPE_WPA2_AES       = ( ENC_WPA2_SECURITY | ENC_AES_ENABLED ),                    /**< WPA2 Security with AES                  */
  ENC_TYPE_WPA2_TKIP      = ( ENC_WPA2_SECURITY | ENC_TKIP_ENABLED ),                   /**< WPA2 Security with TKIP                 */
  ENC_TYPE_WPA2_MIXED     = ( ENC_WPA2_SECURITY | ENC_AES_ENABLED | ENC_TKIP_ENABLED ), /**< WPA2 Security with AES & TKIP           */

  // WPA Enterprise
  ENC_TYPE_WPA_TKIP_ENT   = ( ENC_ENTERPRISE_ENABLED | ENC_WPA_SECURITY  | ENC_TKIP_ENABLED ),
  ENC_TYPE_WPA_AES_ENT    = ( ENC_ENTERPRISE_ENABLED | ENC_WPA_SECURITY  | ENC_AES_ENABLED ),
  ENC_TYPE_WPA_MIXED_ENT  = ( ENC_ENTERPRISE_ENABLED | ENC_WPA_SECURITY  | ENC_AES_ENABLED | ENC_TKIP_ENABLED ),

  // WPA2 Enterprise
  ENC_TYPE_WPA2_TKIP_ENT  = ( ENC_ENTERPRISE_ENABLED | ENC_WPA2_SECURITY | ENC_TKIP_ENABLED ),
  ENC_TYPE_WPA2_AES_ENT   = ( ENC_ENTERPRISE_ENABLED | ENC_WPA2_SECURITY | ENC_AES_ENABLED ),
  ENC_TYPE_WPA2_MIXED_ENT = ( ENC_ENTERPRISE_ENABLED | ENC_WPA2_SECURITY | ENC_AES_ENABLED | ENC_TKIP_ENABLED ),

  // WPS
  ENC_TYPE_WPS_OPEN       = ( ENC_WPS_ENABLED ),                                        /**< WPS with open security                  */
  ENC_TYPE_WPS_SECURE     = ( ENC_WPS_ENABLED | ENC_AES_ENABLED),                       /**< WPS with AES security                   */

  // IBSS
  ENC_TYPE_IBSS_OPEN      = ( ENC_IBSS_ENABLED )
} wl_enc_type_t;


/**
 * @enum err_t
 * @brief Error opcodes
 */
typedef enum
{
  ERROR_NONE                        = 0,
  ERROR_PENDING                     = 1,   /**< Pending */
  ERROR_TIMEOUT                     = 2,   /**< Timeout */
  ERROR_PARTIAL_RESULTS             = 3,   /**< Partial results */
  ERROR_ERROR                       = 4,   /**< Error */
  ERROR_BADARG                      = 5,   /**< Bad Arguments */
  ERROR_BADOPTION                   = 6,   /**< Mode not supported */
  ERROR_UNSUPPORTED                 = 7,   /**< Unsupported function */
  ERROR_OUT_OF_HEAP_SPACE           = 8,   /**< Dynamic memory space exhausted */
  ERROR_NOTUP                       = 9,   /**< Interface is not currently Up */
  ERROR_UNFINISHED                  = 10,   /**< Operation not finished yet */
  ERROR_CONNECTION_LOST             = 11,   /**< Connection to server lost */
  ERROR_NOT_FOUND                   = 12,   /**< Item not found */
  ERROR_PACKET_BUFFER_CORRUPT       = 13,   /**< Packet buffer corrupted */
  ERROR_ROUTING_ERROR               = 14,   /**< Routing error */
  ERROR_BADVALUE                    = 15,   /**< Bad value */
  ERROR_WOULD_BLOCK                 = 16,   /**< Function would block */
  ERROR_ABORTED                     = 17,   /**< Operation aborted */
  ERROR_CONNECTION_RESET            = 18,   /**< Connection has been reset */
  ERROR_CONNECTION_CLOSED           = 19,   /**< Connection is closed */
  ERROR_NOT_CONNECTED               = 20,   /**< Connection is not connected */
  ERROR_ADDRESS_IN_USE              = 21,   /**< Address is in use */
  ERROR_NETWORK_INTERFACE_ERROR     = 22,   /**< Network interface error */
  ERROR_ALREADY_CONNECTED           = 23,   /**< Socket is already connected */
  ERROR_INVALID_INTERFACE           = 24,   /**< Interface specified in invalid */
  ERROR_SOCKET_CREATE_FAIL          = 25,   /**< Socket creation failed */
  ERROR_INVALID_SOCKET              = 26,   /**< Socket is invalid */
  ERROR_CORRUPT_PACKET_BUFFER       = 27,   /**< Packet buffer is corrupted */
  ERROR_UNKNOWN_NETWORK_STACK_ERROR = 28,   /**< Unknown network stack error */
  ERROR_NO_STORED_AP_IN_DCT         = 29,   /**< DCT contains no AP credentials */
  ERROR_STA_JOIN_FAILED             = 30,   /**< Join failed */
  ERROR_PACKET_BUFFER_OVERFLOW      = 31,   /**< Packet buffer overflow */
  ERROR_ALREADY_INITIALIZED         = 32,

  ERROR_WWD_INVALID_KEY             = 1004,  /**< WWD 1004, Invalid AP Password */
  ERROR_WWD_AUTHENTICATION_FAILED   = 1006,  /**< WWD 1006, Authentication error */
  ERROR_WWD_NETWORK_NOT_FOUND       = 1024,  /**< WWD 1024, likely a range issue */
  ERROR_WWD_UNABLE_TO_JOIN          = 1025,  /**< WWD 1025, likely a range issue */
  ERROR_WWD_ACCESS_POINT_NOT_FOUND  = 1066,

  ERROR_TLS_UNTRUSTED_CERTIFICATE   = 5035,  /**< TLS verification failed */

  ERROR_SDEP_INVALIDPARAMETER       = 30002,
} err_t;

/**
 * SDEP Command ID
 */
typedef enum {
  // General Purpose
  SDEP_CMD_RESET               = 0x0001,    ///< HW reset
  SDEP_CMD_FACTORYRESET        = 0x0002,    ///< Factory reset
  SDEP_CMD_DFU                 = 0x0003,    ///< Enter DFU mode
  SDEP_CMD_INFO                = 0x0004,    ///< System information
  SDEP_CMD_NVM_RESET           = 0x0005,    ///< Reset DCT
  SDEP_CMD_ERROR_STRING        = 0x0006,    ///< Get descriptive error string
  SDEP_CMD_COMMAND_STRING      = 0x0007,    ///< Get descriptive SDEP command string

  // Hardware Command
  SDEP_CMD_GPIO                = 0x0100,    ///< Set GPIO
  SDEP_CMD_RANDOMNUMBER        = 0x0101,    ///< Random number

  // SPI Flash Commands
  SDEP_CMD_SFLASHFORMAT        = 0x0200,    ///< Format SPI flash memory
  SDEP_CMD_SFLASHLIST          = 0x0201,    ///< List SPI flash contents

  // DEBUG Commands
  SDEP_CMD_STACKDUMP           = 0x0300,    ///< Dump the stack
  SDEP_CMD_STACKSIZE           = 0x0301,    ///< Get stack size
  SDEP_CMD_HEAPDUMP            = 0x0302,    ///< Dump the heap
  SDEP_CMD_HEAPSIZE            = 0x0303,    ///< Get heap size
  SDEP_CMD_THREADLIST          = 0x0304,    ///< Get thread information

  // WiFi Commands
  SDEP_CMD_SCAN                = 0x0400,    ///< AP scan
  SDEP_CMD_CONNECT             = 0x0401,    ///< Connect to AP
  SDEP_CMD_DISCONNECT          = 0x0402,    ///< Disconnect from AP
  SDEP_CMD_APSTART             = 0x0403,    ///< Start AP
  SDEP_CMD_APSTOP              = 0x0404,    ///< Stop AP
  SDEP_CMD_WIFI_GET_RSSI       = 0x0405,    ///< Get RSSI of current connected signal
  SDEP_CMD_WIFI_PROFILE_ADD    = 0x0406,    ///< Add a network profile
  SDEP_CMD_WIFI_PROFILE_DEL    = 0x0407,    ///< Remove a network profile
  SDEP_CMD_WIFI_PROFILE_CLEAR  = 0x0408,    ///< Clear all network profiles
  SDEP_CMD_WIFI_PROFILE_CHECK  = 0x0409,    ///< Check if a network profile exists
  SDEP_CMD_WIFI_PROFILE_SAVE   = 0x040A,    ///< Save current connected profile to NVM
  SDEP_CMD_WIFI_PROFILE_GET    = 0x040B,    ///< Get AP's profile info
  SDEP_CMD_TLS_DEFAULT_ROOT_CA = 0x040C,    ///< Enable the default Root CA list
  SDEP_CMD_TLS_ADD_ROOT_CA     = 0x040D,    ///< Add an custom ROOT CA to current Chain
  SDEP_CMD_TLS_CLEAR_ROOT_CA   = 0x040E,    ///< Clear the whole ROOT CA chain

  // Gateway Commands
  SDEP_CMD_GET_IPV4_ADDRESS    = 0x0500,    ///< Get IPv4 address from an interface
  SDEP_CMD_GET_IPV6_ADDRESS    = 0x0501,    ///< Get IPv6 address from an interface
  SDEP_CMD_GET_GATEWAY_ADDRESS = 0x0502,    ///< Get IPv6 gateway address
  SDEP_CMD_GET_NETMASK         = 0x0503,    ///< Get IPv4 DNS netmask
  SDEP_CMD_GET_MAC_ADDRESS     = 0x0504,    ///< Get MAC Address

  // Network Commands
  SDEP_CMD_PING                = 0x0600,    ///< Ping
  SDEP_CMD_DNSLOOKUP           = 0x0601,    ///< DNS lookup
  SDEP_CMD_GET_ISO8601_TIME    = 0x0602,    ///< Get time
  SDEP_CMD_GET_UTC_TIME        = 0x0603,    ///< Get UTC time in seconds

  // TCP Commands
  SDEP_CMD_TCP_CONNECT         = 0x0700,    ///< Create TCP stream socket and connect
  SDEP_CMD_TCP_WRITE           = 0x0701,    ///< Write to the TCP stream socket
  SDEP_CMD_TCP_FLUSH           = 0x0702,    ///< Flush TCP stream socket
  SDEP_CMD_TCP_READ            = 0x0703,    ///< Read from the TCP stream socket
  SDEP_CMD_TCP_DISCONNECT      = 0x0704,    ///< Disconnect TCP stream socket
  SDEP_CMD_TCP_AVAILABLE       = 0x0705,    ///< Check if there is data in TCP stream socket
  SDEP_CMD_TCP_PEEK            = 0x0706,    ///< Peek at byte data from TCP stream socket
  SDEP_CMD_TCP_STATUS          = 0x0707,    ///< Get status of TCP stream socket
  SDEP_CMD_TCP_SET_CALLBACK    = 0x0708,    ///< Set callback function for TCP connection
  SDEP_CMD_TCP_LISTEN          = 0x0709,
  SDEP_CMD_TCP_ACCEPT          = 0x070A,
  SDEP_CMD_TCP_PEER_INFO       = 0x070B,

  // UDP Commands
  SDEP_CMD_UDP_CREATE          = 0x0800,    ///< Create UDP socket
  SDEP_CMD_UDP_WRITE           = 0x0801,    ///< Write to the UDP socket
  SDEP_CMD_UDP_FLUSH           = 0x0802,    ///< Flush UDP stream socket
  SDEP_CMD_UDP_READ            = 0x0803,    ///< Read from the UDP stream socket
  SDEP_CMD_UDP_CLOSE           = 0x0804,    ///< Close UDP stream socket
  SDEP_CMD_UDP_AVAILABLE       = 0x0805,    ///< Check if there is data in UDP stream socket
  SDEP_CMD_UDP_PEEK            = 0x0806,    ///< Peek at byte data from UDP stream socket
  SDEP_CMD_UDP_PACKET_INFO     = 0x0807,    ///< Get packet info of UDP stream socket

  // MQTT Commands
  SDEP_CMD_MQTTCONNECT         = 0x0900,    ///< Connect to a broker
  SDEP_CMD_MQTTDISCONNECT      = 0x0901,    ///< Disconnect from a broker
  SDEP_CMD_MQTTPUBLISH         = 0x0902,    ///< Publish a message to a topic
  SDEP_CMD_MQTTSUBSCRIBE       = 0x0903,    ///< Subscribe to a topic
  SDEP_CMD_MQTTUNSUBSCRIBE     = 0x0904,    ///< Unsubscribe from a topic

  // Email
  SDEP_CMD_EMAIL_SEND          = 0x0A00,    ///< Send an email

  // Cryptography
  SDEP_CMD_HMAC_SHA1_START     = 0x0B00,    ///< HMAC SHA1 Start
  SDEP_CMD_HMAC_SHA1_UPDATE    = 0x0B01,    ///< HMAC SHA1 Update
  SDEP_CMD_HMAC_SHA1_FINISH    = 0x0B02,    ///< HMAC SHA1 Finish
  SDEP_CMD_BASE64_ENCODE       = 0x0B03,    ///< Base64 encode
  SDEP_CMD_BASE64_DECODE       = 0x0B04,    ///< Base64 decode
} sdep_command_t;

enum
{
  TLS_NO_VERIFICATION       = 0,
  TLS_VERIFICATION_OPTIONAL = 1,
  TLS_VERIFICATION_REQUIRED = 2,
};

enum
{
  TIMEOUT_NO_WAIT = 0,
  TIMEOUT_FOREVER = UINT32_MAX
};

#ifdef __cplusplus
 }
#endif

#endif /* _ADAFRUIT_CONSTANTS_H_ */
