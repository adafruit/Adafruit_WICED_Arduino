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
  WIFI_INTERFACE_SOFTAP  = 1,
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
  SDEP_CMD_RESET                 = 0x0001,    ///< HW reset
  SDEP_CMD_FACTORYRESET          = 0x0002,    ///< Factory reset
  SDEP_CMD_DFU                   = 0x0003,    ///< Enter DFU mode
  SDEP_CMD_INFO                  = 0x0004,    ///< System information
  SDEP_CMD_NVM_RESET             = 0x0005,    ///< Reset DCT
  SDEP_CMD_ERROR_STRING          = 0x0006,    ///< Get descriptive error string
  SDEP_CMD_COMMAND_STRING        = 0x0007,    ///< Get descriptive SDEP command string
  SDEP_CMD_SFLASH_ERASEALL       = 0x0008,    ///< Erase SPI flash memory

  // Hardware Command
  SDEP_CMD_GPIO                  = 0x0100,    ///< Set GPIO
  SDEP_CMD_RANDOMNUMBER          = 0x0101,    ///< Random number
  SDEP_CMD_USBMSC_FORCE_REFRESH  = 0x0102,    ///< Force USB MSC host to refresh

  // DEBUG Commands
  SDEP_CMD_STACKDUMP             = 0x0300,    ///< Dump the stack
  SDEP_CMD_STACKSIZE             = 0x0301,    ///< Get stack size
  SDEP_CMD_HEAPDUMP              = 0x0302,    ///< Dump the heap
  SDEP_CMD_HEAPSIZE              = 0x0303,    ///< Get heap size
  SDEP_CMD_THREADLIST_PRINT      = 0x0304,    ///< Print thread information
  SDEP_CMD_THREAD_INFO           = 0x0305,    ///< Get thread info
  SDEP_CMD_ARDUINO_HEAPSIZE      = 0x0306,    ///< Get Arduino heap size

  // WiFi Commands
  SDEP_CMD_SCAN                  = 0x0400,    ///< AP scan
  SDEP_CMD_CONNECT               = 0x0401,    ///< Connect to AP
  SDEP_CMD_DISCONNECT            = 0x0402,    ///< Disconnect from AP
  // 0x0403,    ///< Reserved1
  SDEP_CMD_WIFI_COUNTRY          = 0x0404,    ///< Set/Get Wifi country code
  SDEP_CMD_WIFI_GET_RSSI         = 0x0405,    ///< Get RSSI of current connected signal
  SDEP_CMD_WIFI_PROFILE_ADD      = 0x0406,    ///< Add a network profile
  SDEP_CMD_WIFI_PROFILE_DEL      = 0x0407,    ///< Remove a network profile
  SDEP_CMD_WIFI_PROFILE_CLEAR    = 0x0408,    ///< Clear all network profiles
  SDEP_CMD_WIFI_PROFILE_CHECK    = 0x0409,    ///< Check if a network profile exists
  SDEP_CMD_WIFI_PROFILE_SAVE     = 0x040A,    ///< Save current connected profile to NVM
  SDEP_CMD_WIFI_PROFILE_GET      = 0x040B,    ///< Get AP's profile info
  SDEP_CMD_TLS_DEFAULT_ROOT_CA   = 0x040C,    ///< Enable the default Root CA list
  SDEP_CMD_TLS_ADD_ROOT_CA       = 0x040D,    ///< Add an custom ROOT CA to current Chain
  SDEP_CMD_TLS_CLEAR_ROOT_CA     = 0x040E,    ///< Clear the whole ROOT CA chain

  // Gateway Commands
  SDEP_CMD_GET_IPV4_ADDRESS      = 0x0500,    ///< Get IPv4 address from an interface
  SDEP_CMD_GET_IPV6_ADDRESS      = 0x0501,    ///< Get IPv6 address from an interface
  SDEP_CMD_GET_GATEWAY_ADDRESS   = 0x0502,    ///< Get IPv6 gateway address
  SDEP_CMD_GET_NETMASK           = 0x0503,    ///< Get IPv4 DNS netmask
  SDEP_CMD_GET_MAC_ADDRESS       = 0x0504,    ///< Get MAC Address

  // Network Commands
  SDEP_CMD_PING                  = 0x0600,    ///< Ping
  SDEP_CMD_DNSLOOKUP             = 0x0601,    ///< DNS lookup
  SDEP_CMD_GET_ISO8601_TIME      = 0x0602,    ///< Get time
  SDEP_CMD_GET_UTC_TIME          = 0x0603,    ///< Get UTC time in seconds
  SDEP_CMD_SET_NTP_SERVER        = 0x0604,    ///< Set NTP Server, NULL to disable

  // TCP Commands
  SDEP_CMD_TCP_CONNECT           = 0x0700,    ///< Create TCP stream socket and connect
  SDEP_CMD_TCP_WRITE             = 0x0701,    ///< Write to the TCP stream socket
  SDEP_CMD_TCP_FLUSH             = 0x0702,    ///< Flush TCP stream socket
  SDEP_CMD_TCP_READ              = 0x0703,    ///< Read from the TCP stream socket
  SDEP_CMD_TCP_DISCONNECT        = 0x0704,    ///< Disconnect TCP stream socket
  SDEP_CMD_TCP_AVAILABLE         = 0x0705,    ///< Check if there is data in TCP stream socket
  SDEP_CMD_TCP_PEEK              = 0x0706,    ///< Peek at byte data from TCP stream socket
  SDEP_CMD_TCP_STATUS            = 0x0707,    ///< Get status of TCP stream socket
  SDEP_CMD_TCP_SET_CALLBACK      = 0x0708,    ///< Set callback function for TCP connection
  SDEP_CMD_TCP_LISTEN            = 0x0709,
  SDEP_CMD_TCP_ACCEPT            = 0x070A,
  SDEP_CMD_TCP_PEER_INFO         = 0x070B,

  // UDP Commands
  SDEP_CMD_UDP_CREATE            = 0x0800,    ///< Create UDP socket
  SDEP_CMD_UDP_WRITE             = 0x0801,    ///< Write to the UDP socket
  SDEP_CMD_UDP_FLUSH             = 0x0802,    ///< Flush UDP stream socket
  SDEP_CMD_UDP_READ              = 0x0803,    ///< Read from the UDP stream socket
  SDEP_CMD_UDP_CLOSE             = 0x0804,    ///< Close UDP stream socket
  SDEP_CMD_UDP_AVAILABLE         = 0x0805,    ///< Check if there is data in UDP stream socket
  SDEP_CMD_UDP_PEEK              = 0x0806,    ///< Peek at byte data from UDP stream socket
  SDEP_CMD_UDP_PACKET_INFO       = 0x0807,    ///< Get packet info of UDP stream socket

  // MQTT Commands
  SDEP_CMD_MQTTCONNECT           = 0x0900,    ///< Connect to a broker
  SDEP_CMD_MQTTDISCONNECT        = 0x0901,    ///< Disconnect from a broker
  SDEP_CMD_MQTTPUBLISH           = 0x0902,    ///< Publish a message to a topic
  SDEP_CMD_MQTTSUBSCRIBE         = 0x0903,    ///< Subscribe to a topic
  SDEP_CMD_MQTTUNSUBSCRIBE       = 0x0904,    ///< Unsubscribe from a topic

  // Email
  SDEP_CMD_EMAIL_SEND            = 0x0A00,    ///< Send an email

  // Cryptography
  SDEP_CMD_HMAC_SHA1_START       = 0x0B00,    ///< HMAC SHA1 Start
  SDEP_CMD_HMAC_SHA1_UPDATE      = 0x0B01,    ///< HMAC SHA1 Update
  SDEP_CMD_HMAC_SHA1_FINISH      = 0x0B02,    ///< HMAC SHA1 Finish
  SDEP_CMD_BASE64_ENCODE         = 0x0B03,    ///< Base64 encode
  SDEP_CMD_BASE64_DECODE         = 0x0B04,    ///< Base64 decode

  // SoftAP
  SDEP_CMD_SOFTAP_START          = 0x0C00,    ///< Start SoftAP
  SDEP_CMD_SOFTAP_STOP           = 0x0C01,    ///< Stop SoftAP
  SDEP_CMD_SOFTAP_CLIENT_RSSI    = 0x0C02,    ///< SoftAP get client RSSI

  // HTTP Server
  SDEP_CMD_HTTPSERVER_START      = 0x0D00,    ///< Start HTTP Server
  SDEP_CMD_HTTPSERVER_STOP       = 0x0D01,    ///< Stop HTTP Server
  SDEP_CMD_HTTPSERVER_RESP_WRITE = 0x0D02,    ///< HTTP Server Response Write

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

#define MK_CNTRY( a, b, rev )  (((unsigned char)(a)) + (((unsigned char)(b))<<8) + (((unsigned short)(rev))<<16))

/**
 * Enumerated list of country codes
 */
typedef enum
{
    WIFI_COUNTRY_AFGHANISTAN                                     = MK_CNTRY( 'A', 'F', 0 ),             /* AF Afghanistan */
    WIFI_COUNTRY_ALBANIA                                         = MK_CNTRY( 'A', 'L', 0 ),             /* AL Albania */
    WIFI_COUNTRY_ALGERIA                                         = MK_CNTRY( 'D', 'Z', 0 ),             /* DZ Algeria */
    WIFI_COUNTRY_AMERICAN_SAMOA                                  = MK_CNTRY( 'A', 'S', 0 ),             /* AS American_Samoa */
    WIFI_COUNTRY_ANGOLA                                          = MK_CNTRY( 'A', 'O', 0 ),             /* AO Angola */
    WIFI_COUNTRY_ANGUILLA                                        = MK_CNTRY( 'A', 'I', 0 ),             /* AI Anguilla */
    WIFI_COUNTRY_ANTIGUA_AND_BARBUDA                             = MK_CNTRY( 'A', 'G', 0 ),             /* AG Antigua_and_Barbuda */
    WIFI_COUNTRY_ARGENTINA                                       = MK_CNTRY( 'A', 'R', 0 ),             /* AR Argentina */
    WIFI_COUNTRY_ARMENIA                                         = MK_CNTRY( 'A', 'M', 0 ),             /* AM Armenia */
    WIFI_COUNTRY_ARUBA                                           = MK_CNTRY( 'A', 'W', 0 ),             /* AW Aruba */
    WIFI_COUNTRY_AUSTRALIA                                       = MK_CNTRY( 'A', 'U', 0 ),             /* AU Australia */
    WIFI_COUNTRY_AUSTRIA                                         = MK_CNTRY( 'A', 'T', 0 ),             /* AT Austria */
    WIFI_COUNTRY_AZERBAIJAN                                      = MK_CNTRY( 'A', 'Z', 0 ),             /* AZ Azerbaijan */
    WIFI_COUNTRY_BAHAMAS                                         = MK_CNTRY( 'B', 'S', 0 ),             /* BS Bahamas */
    WIFI_COUNTRY_BAHRAIN                                         = MK_CNTRY( 'B', 'H', 0 ),             /* BH Bahrain */
    WIFI_COUNTRY_BAKER_ISLAND                                    = MK_CNTRY( '0', 'B', 0 ),             /* 0B Baker_Island */
    WIFI_COUNTRY_BANGLADESH                                      = MK_CNTRY( 'B', 'D', 0 ),             /* BD Bangladesh */
    WIFI_COUNTRY_BARBADOS                                        = MK_CNTRY( 'B', 'B', 0 ),             /* BB Barbados */
    WIFI_COUNTRY_BELARUS                                         = MK_CNTRY( 'B', 'Y', 0 ),             /* BY Belarus */
    WIFI_COUNTRY_BELGIUM                                         = MK_CNTRY( 'B', 'E', 0 ),             /* BE Belgium */
    WIFI_COUNTRY_BELIZE                                          = MK_CNTRY( 'B', 'Z', 0 ),             /* BZ Belize */
    WIFI_COUNTRY_BENIN                                           = MK_CNTRY( 'B', 'J', 0 ),             /* BJ Benin */
    WIFI_COUNTRY_BERMUDA                                         = MK_CNTRY( 'B', 'M', 0 ),             /* BM Bermuda */
    WIFI_COUNTRY_BHUTAN                                          = MK_CNTRY( 'B', 'T', 0 ),             /* BT Bhutan */
    WIFI_COUNTRY_BOLIVIA                                         = MK_CNTRY( 'B', 'O', 0 ),             /* BO Bolivia */
    WIFI_COUNTRY_BOSNIA_AND_HERZEGOVINA                          = MK_CNTRY( 'B', 'A', 0 ),             /* BA Bosnia_and_Herzegovina */
    WIFI_COUNTRY_BOTSWANA                                        = MK_CNTRY( 'B', 'W', 0 ),             /* BW Botswana */
    WIFI_COUNTRY_BRAZIL                                          = MK_CNTRY( 'B', 'R', 0 ),             /* BR Brazil */
    WIFI_COUNTRY_BRITISH_INDIAN_OCEAN_TERRITORY                  = MK_CNTRY( 'I', 'O', 0 ),             /* IO British_Indian_Ocean_Territory */
    WIFI_COUNTRY_BRUNEI_DARUSSALAM                               = MK_CNTRY( 'B', 'N', 0 ),             /* BN Brunei_Darussalam */
    WIFI_COUNTRY_BULGARIA                                        = MK_CNTRY( 'B', 'G', 0 ),             /* BG Bulgaria */
    WIFI_COUNTRY_BURKINA_FASO                                    = MK_CNTRY( 'B', 'F', 0 ),             /* BF Burkina_Faso */
    WIFI_COUNTRY_BURUNDI                                         = MK_CNTRY( 'B', 'I', 0 ),             /* BI Burundi */
    WIFI_COUNTRY_CAMBODIA                                        = MK_CNTRY( 'K', 'H', 0 ),             /* KH Cambodia */
    WIFI_COUNTRY_CAMEROON                                        = MK_CNTRY( 'C', 'M', 0 ),             /* CM Cameroon */
    WIFI_COUNTRY_CANADA                                          = MK_CNTRY( 'C', 'A', 0 ),             /* CA Canada */
    WIFI_COUNTRY_CAPE_VERDE                                      = MK_CNTRY( 'C', 'V', 0 ),             /* CV Cape_Verde */
    WIFI_COUNTRY_CAYMAN_ISLANDS                                  = MK_CNTRY( 'K', 'Y', 0 ),             /* KY Cayman_Islands */
    WIFI_COUNTRY_CENTRAL_AFRICAN_REPUBLIC                        = MK_CNTRY( 'C', 'F', 0 ),             /* CF Central_African_Republic */
    WIFI_COUNTRY_CHAD                                            = MK_CNTRY( 'T', 'D', 0 ),             /* TD Chad */
    WIFI_COUNTRY_CHILE                                           = MK_CNTRY( 'C', 'L', 0 ),             /* CL Chile */
    WIFI_COUNTRY_CHINA                                           = MK_CNTRY( 'C', 'N', 0 ),             /* CN China */
    WIFI_COUNTRY_CHRISTMAS_ISLAND                                = MK_CNTRY( 'C', 'X', 0 ),             /* CX Christmas_Island */
    WIFI_COUNTRY_COLOMBIA                                        = MK_CNTRY( 'C', 'O', 0 ),             /* CO Colombia */
    WIFI_COUNTRY_COMOROS                                         = MK_CNTRY( 'K', 'M', 0 ),             /* KM Comoros */
    WIFI_COUNTRY_CONGO                                           = MK_CNTRY( 'C', 'G', 0 ),             /* CG Congo */
    WIFI_COUNTRY_CONGO_THE_DEMOCRATIC_REPUBLIC_OF_THE            = MK_CNTRY( 'C', 'D', 0 ),             /* CD Congo,_The_Democratic_Republic_Of_The */
    WIFI_COUNTRY_COSTA_RICA                                      = MK_CNTRY( 'C', 'R', 0 ),             /* CR Costa_Rica */
    WIFI_COUNTRY_COTE_DIVOIRE                                    = MK_CNTRY( 'C', 'I', 0 ),             /* CI Cote_D'ivoire */
    WIFI_COUNTRY_CROATIA                                         = MK_CNTRY( 'H', 'R', 0 ),             /* HR Croatia */
    WIFI_COUNTRY_CUBA                                            = MK_CNTRY( 'C', 'U', 0 ),             /* CU Cuba */
    WIFI_COUNTRY_CYPRUS                                          = MK_CNTRY( 'C', 'Y', 0 ),             /* CY Cyprus */
    WIFI_COUNTRY_CZECH_REPUBLIC                                  = MK_CNTRY( 'C', 'Z', 0 ),             /* CZ Czech_Republic */
    WIFI_COUNTRY_DENMARK                                         = MK_CNTRY( 'D', 'K', 0 ),             /* DK Denmark */
    WIFI_COUNTRY_DJIBOUTI                                        = MK_CNTRY( 'D', 'J', 0 ),             /* DJ Djibouti */
    WIFI_COUNTRY_DOMINICA                                        = MK_CNTRY( 'D', 'M', 0 ),             /* DM Dominica */
    WIFI_COUNTRY_DOMINICAN_REPUBLIC                              = MK_CNTRY( 'D', 'O', 0 ),             /* DO Dominican_Republic */
    WIFI_COUNTRY_DOWN_UNDER                                      = MK_CNTRY( 'A', 'U', 0 ),             /* AU G'Day mate! */
    WIFI_COUNTRY_ECUADOR                                         = MK_CNTRY( 'E', 'C', 0 ),             /* EC Ecuador */
    WIFI_COUNTRY_EGYPT                                           = MK_CNTRY( 'E', 'G', 0 ),             /* EG Egypt */
    WIFI_COUNTRY_EL_SALVADOR                                     = MK_CNTRY( 'S', 'V', 0 ),             /* SV El_Salvador */
    WIFI_COUNTRY_EQUATORIAL_GUINEA                               = MK_CNTRY( 'G', 'Q', 0 ),             /* GQ Equatorial_Guinea */
    WIFI_COUNTRY_ERITREA                                         = MK_CNTRY( 'E', 'R', 0 ),             /* ER Eritrea */
    WIFI_COUNTRY_ESTONIA                                         = MK_CNTRY( 'E', 'E', 0 ),             /* EE Estonia */
    WIFI_COUNTRY_ETHIOPIA                                        = MK_CNTRY( 'E', 'T', 0 ),             /* ET Ethiopia */
    WIFI_COUNTRY_FALKLAND_ISLANDS_MALVINAS                       = MK_CNTRY( 'F', 'K', 0 ),             /* FK Falkland_Islands_(Malvinas) */
    WIFI_COUNTRY_FAROE_ISLANDS                                   = MK_CNTRY( 'F', 'O', 0 ),             /* FO Faroe_Islands */
    WIFI_COUNTRY_FIJI                                            = MK_CNTRY( 'F', 'J', 0 ),             /* FJ Fiji */
    WIFI_COUNTRY_FINLAND                                         = MK_CNTRY( 'F', 'I', 0 ),             /* FI Finland */
    WIFI_COUNTRY_FRANCE                                          = MK_CNTRY( 'F', 'R', 0 ),             /* FR France */
    WIFI_COUNTRY_FRENCH_GUINA                                    = MK_CNTRY( 'G', 'F', 0 ),             /* GF French_Guina */
    WIFI_COUNTRY_FRENCH_POLYNESIA                                = MK_CNTRY( 'P', 'F', 0 ),             /* PF French_Polynesia */
    WIFI_COUNTRY_FRENCH_SOUTHERN_TERRITORIES                     = MK_CNTRY( 'T', 'F', 0 ),             /* TF French_Southern_Territories */
    WIFI_COUNTRY_GABON                                           = MK_CNTRY( 'G', 'A', 0 ),             /* GA Gabon */
    WIFI_COUNTRY_GAMBIA                                          = MK_CNTRY( 'G', 'M', 0 ),             /* GM Gambia */
    WIFI_COUNTRY_GEORGIA                                         = MK_CNTRY( 'G', 'E', 0 ),             /* GE Georgia */
    WIFI_COUNTRY_GERMANY                                         = MK_CNTRY( 'D', 'E', 0 ),             /* DE Germany */
    WIFI_COUNTRY_GHANA                                           = MK_CNTRY( 'G', 'H', 0 ),             /* GH Ghana */
    WIFI_COUNTRY_GIBRALTAR                                       = MK_CNTRY( 'G', 'I', 0 ),             /* GI Gibraltar */
    WIFI_COUNTRY_GREECE                                          = MK_CNTRY( 'G', 'R', 0 ),             /* GR Greece */
    WIFI_COUNTRY_GRENADA                                         = MK_CNTRY( 'G', 'D', 0 ),             /* GD Grenada */
    WIFI_COUNTRY_GUADELOUPE                                      = MK_CNTRY( 'G', 'P', 0 ),             /* GP Guadeloupe */
    WIFI_COUNTRY_GUAM                                            = MK_CNTRY( 'G', 'U', 0 ),             /* GU Guam */
    WIFI_COUNTRY_GUATEMALA                                       = MK_CNTRY( 'G', 'T', 0 ),             /* GT Guatemala */
    WIFI_COUNTRY_GUERNSEY                                        = MK_CNTRY( 'G', 'G', 0 ),             /* GG Guernsey */
    WIFI_COUNTRY_GUINEA                                          = MK_CNTRY( 'G', 'N', 0 ),             /* GN Guinea */
    WIFI_COUNTRY_GUINEA_BISSAU                                   = MK_CNTRY( 'G', 'W', 0 ),             /* GW Guinea-bissau */
    WIFI_COUNTRY_GUYANA                                          = MK_CNTRY( 'G', 'Y', 0 ),             /* GY Guyana */
    WIFI_COUNTRY_HAITI                                           = MK_CNTRY( 'H', 'T', 0 ),             /* HT Haiti */
    WIFI_COUNTRY_HOLY_SEE_VATICAN_CITY_STATE                     = MK_CNTRY( 'V', 'A', 0 ),             /* VA Holy_See_(Vatican_City_State) */
    WIFI_COUNTRY_HONDURAS                                        = MK_CNTRY( 'H', 'N', 0 ),             /* HN Honduras */
    WIFI_COUNTRY_HONG_KONG                                       = MK_CNTRY( 'H', 'K', 0 ),             /* HK Hong_Kong */
    WIFI_COUNTRY_HUNGARY                                         = MK_CNTRY( 'H', 'U', 0 ),             /* HU Hungary */
    WIFI_COUNTRY_ICELAND                                         = MK_CNTRY( 'I', 'S', 0 ),             /* IS Iceland */
    WIFI_COUNTRY_INDIA                                           = MK_CNTRY( 'I', 'N', 0 ),             /* IN India */
    WIFI_COUNTRY_INDONESIA                                       = MK_CNTRY( 'I', 'D', 0 ),             /* ID Indonesia */
    WIFI_COUNTRY_IRAN_ISLAMIC_REPUBLIC_OF                        = MK_CNTRY( 'I', 'R', 0 ),             /* IR Iran,_Islamic_Republic_Of */
    WIFI_COUNTRY_IRAQ                                            = MK_CNTRY( 'I', 'Q', 0 ),             /* IQ Iraq */
    WIFI_COUNTRY_IRELAND                                         = MK_CNTRY( 'I', 'E', 0 ),             /* IE Ireland */
    WIFI_COUNTRY_ISRAEL                                          = MK_CNTRY( 'I', 'L', 0 ),             /* IL Israel */
    WIFI_COUNTRY_ITALY                                           = MK_CNTRY( 'I', 'T', 0 ),             /* IT Italy */
    WIFI_COUNTRY_JAMAICA                                         = MK_CNTRY( 'J', 'M', 0 ),             /* JM Jamaica */
    WIFI_COUNTRY_JAPAN                                           = MK_CNTRY( 'J', 'P', 0 ),             /* JP Japan */
    WIFI_COUNTRY_JERSEY                                          = MK_CNTRY( 'J', 'E', 0 ),             /* JE Jersey */
    WIFI_COUNTRY_JORDAN                                          = MK_CNTRY( 'J', 'O', 0 ),             /* JO Jordan */
    WIFI_COUNTRY_KAZAKHSTAN                                      = MK_CNTRY( 'K', 'Z', 0 ),             /* KZ Kazakhstan */
    WIFI_COUNTRY_KENYA                                           = MK_CNTRY( 'K', 'E', 0 ),             /* KE Kenya */
    WIFI_COUNTRY_KIRIBATI                                        = MK_CNTRY( 'K', 'I', 0 ),             /* KI Kiribati */
    WIFI_COUNTRY_KOREA_REPUBLIC_OF                               = MK_CNTRY( 'K', 'R', 1 ),             /* KR Korea,_Republic_Of */
    WIFI_COUNTRY_KOSOVO                                          = MK_CNTRY( '0', 'A', 0 ),             /* 0A Kosovo */
    WIFI_COUNTRY_KUWAIT                                          = MK_CNTRY( 'K', 'W', 0 ),             /* KW Kuwait */
    WIFI_COUNTRY_KYRGYZSTAN                                      = MK_CNTRY( 'K', 'G', 0 ),             /* KG Kyrgyzstan */
    WIFI_COUNTRY_LAO_PEOPLES_DEMOCRATIC_REPUBIC                  = MK_CNTRY( 'L', 'A', 0 ),             /* LA Lao_People's_Democratic_Repubic */
    WIFI_COUNTRY_LATVIA                                          = MK_CNTRY( 'L', 'V', 0 ),             /* LV Latvia */
    WIFI_COUNTRY_LEBANON                                         = MK_CNTRY( 'L', 'B', 0 ),             /* LB Lebanon */
    WIFI_COUNTRY_LESOTHO                                         = MK_CNTRY( 'L', 'S', 0 ),             /* LS Lesotho */
    WIFI_COUNTRY_LIBERIA                                         = MK_CNTRY( 'L', 'R', 0 ),             /* LR Liberia */
    WIFI_COUNTRY_LIBYAN_ARAB_JAMAHIRIYA                          = MK_CNTRY( 'L', 'Y', 0 ),             /* LY Libyan_Arab_Jamahiriya */
    WIFI_COUNTRY_LIECHTENSTEIN                                   = MK_CNTRY( 'L', 'I', 0 ),             /* LI Liechtenstein */
    WIFI_COUNTRY_LITHUANIA                                       = MK_CNTRY( 'L', 'T', 0 ),             /* LT Lithuania */
    WIFI_COUNTRY_LUXEMBOURG                                      = MK_CNTRY( 'L', 'U', 0 ),             /* LU Luxembourg */
    WIFI_COUNTRY_MACAO                                           = MK_CNTRY( 'M', 'O', 0 ),             /* MO Macao */
    WIFI_COUNTRY_MACEDONIA_FORMER_YUGOSLAV_REPUBLIC_OF           = MK_CNTRY( 'M', 'K', 0 ),             /* MK Macedonia,_Former_Yugoslav_Republic_Of */
    WIFI_COUNTRY_MADAGASCAR                                      = MK_CNTRY( 'M', 'G', 0 ),             /* MG Madagascar */
    WIFI_COUNTRY_MALAWI                                          = MK_CNTRY( 'M', 'W', 0 ),             /* MW Malawi */
    WIFI_COUNTRY_MALAYSIA                                        = MK_CNTRY( 'M', 'Y', 0 ),             /* MY Malaysia */
    WIFI_COUNTRY_MALDIVES                                        = MK_CNTRY( 'M', 'V', 0 ),             /* MV Maldives */
    WIFI_COUNTRY_MALI                                            = MK_CNTRY( 'M', 'L', 0 ),             /* ML Mali */
    WIFI_COUNTRY_MALTA                                           = MK_CNTRY( 'M', 'T', 0 ),             /* MT Malta */
    WIFI_COUNTRY_MAN_ISLE_OF                                     = MK_CNTRY( 'I', 'M', 0 ),             /* IM Man,_Isle_Of */
    WIFI_COUNTRY_MARTINIQUE                                      = MK_CNTRY( 'M', 'Q', 0 ),             /* MQ Martinique */
    WIFI_COUNTRY_MAURITANIA                                      = MK_CNTRY( 'M', 'R', 0 ),             /* MR Mauritania */
    WIFI_COUNTRY_MAURITIUS                                       = MK_CNTRY( 'M', 'U', 0 ),             /* MU Mauritius */
    WIFI_COUNTRY_MAYOTTE                                         = MK_CNTRY( 'Y', 'T', 0 ),             /* YT Mayotte */
    WIFI_COUNTRY_MEXICO                                          = MK_CNTRY( 'M', 'X', 0 ),             /* MX Mexico */
    WIFI_COUNTRY_MICRONESIA_FEDERATED_STATES_OF                  = MK_CNTRY( 'F', 'M', 0 ),             /* FM Micronesia,_Federated_States_Of */
    WIFI_COUNTRY_MOLDOVA_REPUBLIC_OF                             = MK_CNTRY( 'M', 'D', 0 ),             /* MD Moldova,_Republic_Of */
    WIFI_COUNTRY_MONACO                                          = MK_CNTRY( 'M', 'C', 0 ),             /* MC Monaco */
    WIFI_COUNTRY_MONGOLIA                                        = MK_CNTRY( 'M', 'N', 0 ),             /* MN Mongolia */
    WIFI_COUNTRY_MONTENEGRO                                      = MK_CNTRY( 'M', 'E', 0 ),             /* ME Montenegro */
    WIFI_COUNTRY_MONTSERRAT                                      = MK_CNTRY( 'M', 'S', 0 ),             /* MS Montserrat */
    WIFI_COUNTRY_MOROCCO                                         = MK_CNTRY( 'M', 'A', 0 ),             /* MA Morocco */
    WIFI_COUNTRY_MOZAMBIQUE                                      = MK_CNTRY( 'M', 'Z', 0 ),             /* MZ Mozambique */
    WIFI_COUNTRY_MYANMAR                                         = MK_CNTRY( 'M', 'M', 0 ),             /* MM Myanmar */
    WIFI_COUNTRY_NAMIBIA                                         = MK_CNTRY( 'N', 'A', 0 ),             /* NA Namibia */
    WIFI_COUNTRY_NAURU                                           = MK_CNTRY( 'N', 'R', 0 ),             /* NR Nauru */
    WIFI_COUNTRY_NEPAL                                           = MK_CNTRY( 'N', 'P', 0 ),             /* NP Nepal */
    WIFI_COUNTRY_NETHERLANDS                                     = MK_CNTRY( 'N', 'L', 0 ),             /* NL Netherlands */
    WIFI_COUNTRY_NETHERLANDS_ANTILLES                            = MK_CNTRY( 'A', 'N', 0 ),             /* AN Netherlands_Antilles */
    WIFI_COUNTRY_NEW_CALEDONIA                                   = MK_CNTRY( 'N', 'C', 0 ),             /* NC New_Caledonia */
    WIFI_COUNTRY_NEW_ZEALAND                                     = MK_CNTRY( 'N', 'Z', 0 ),             /* NZ New_Zealand */
    WIFI_COUNTRY_NICARAGUA                                       = MK_CNTRY( 'N', 'I', 0 ),             /* NI Nicaragua */
    WIFI_COUNTRY_NIGER                                           = MK_CNTRY( 'N', 'E', 0 ),             /* NE Niger */
    WIFI_COUNTRY_NIGERIA                                         = MK_CNTRY( 'N', 'G', 0 ),             /* NG Nigeria */
    WIFI_COUNTRY_NORFOLK_ISLAND                                  = MK_CNTRY( 'N', 'F', 0 ),             /* NF Norfolk_Island */
    WIFI_COUNTRY_NORTHERN_MARIANA_ISLANDS                        = MK_CNTRY( 'M', 'P', 0 ),             /* MP Northern_Mariana_Islands */
    WIFI_COUNTRY_NORWAY                                          = MK_CNTRY( 'N', 'O', 0 ),             /* NO Norway */
    WIFI_COUNTRY_OMAN                                            = MK_CNTRY( 'O', 'M', 0 ),             /* OM Oman */
    WIFI_COUNTRY_PAKISTAN                                        = MK_CNTRY( 'P', 'K', 0 ),             /* PK Pakistan */
    WIFI_COUNTRY_PALAU                                           = MK_CNTRY( 'P', 'W', 0 ),             /* PW Palau */
    WIFI_COUNTRY_PANAMA                                          = MK_CNTRY( 'P', 'A', 0 ),             /* PA Panama */
    WIFI_COUNTRY_PAPUA_NEW_GUINEA                                = MK_CNTRY( 'P', 'G', 0 ),             /* PG Papua_New_Guinea */
    WIFI_COUNTRY_PARAGUAY                                        = MK_CNTRY( 'P', 'Y', 0 ),             /* PY Paraguay */
    WIFI_COUNTRY_PERU                                            = MK_CNTRY( 'P', 'E', 0 ),             /* PE Peru */
    WIFI_COUNTRY_PHILIPPINES                                     = MK_CNTRY( 'P', 'H', 0 ),             /* PH Philippines */
    WIFI_COUNTRY_POLAND                                          = MK_CNTRY( 'P', 'L', 0 ),             /* PL Poland */
    WIFI_COUNTRY_PORTUGAL                                        = MK_CNTRY( 'P', 'T', 0 ),             /* PT Portugal */
    WIFI_COUNTRY_PUETO_RICO                                      = MK_CNTRY( 'P', 'R', 0 ),             /* PR Pueto_Rico */
    WIFI_COUNTRY_QATAR                                           = MK_CNTRY( 'Q', 'A', 0 ),             /* QA Qatar */
    WIFI_COUNTRY_REUNION                                         = MK_CNTRY( 'R', 'E', 0 ),             /* RE Reunion */
    WIFI_COUNTRY_ROMANIA                                         = MK_CNTRY( 'R', 'O', 0 ),             /* RO Romania */
    WIFI_COUNTRY_RUSSIAN_FEDERATION                              = MK_CNTRY( 'R', 'U', 0 ),             /* RU Russian_Federation */
    WIFI_COUNTRY_RWANDA                                          = MK_CNTRY( 'R', 'W', 0 ),             /* RW Rwanda */
    WIFI_COUNTRY_SAINT_KITTS_AND_NEVIS                           = MK_CNTRY( 'K', 'N', 0 ),             /* KN Saint_Kitts_and_Nevis */
    WIFI_COUNTRY_SAINT_LUCIA                                     = MK_CNTRY( 'L', 'C', 0 ),             /* LC Saint_Lucia */
    WIFI_COUNTRY_SAINT_PIERRE_AND_MIQUELON                       = MK_CNTRY( 'P', 'M', 0 ),             /* PM Saint_Pierre_and_Miquelon */
    WIFI_COUNTRY_SAINT_VINCENT_AND_THE_GRENADINES                = MK_CNTRY( 'V', 'C', 0 ),             /* VC Saint_Vincent_and_The_Grenadines */
    WIFI_COUNTRY_SAMOA                                           = MK_CNTRY( 'W', 'S', 0 ),             /* WS Samoa */
    WIFI_COUNTRY_SANIT_MARTIN_SINT_MARTEEN                       = MK_CNTRY( 'M', 'F', 0 ),             /* MF Sanit_Martin_/_Sint_Marteen */
    WIFI_COUNTRY_SAO_TOME_AND_PRINCIPE                           = MK_CNTRY( 'S', 'T', 0 ),             /* ST Sao_Tome_and_Principe */
    WIFI_COUNTRY_SAUDI_ARABIA                                    = MK_CNTRY( 'S', 'A', 0 ),             /* SA Saudi_Arabia */
    WIFI_COUNTRY_SENEGAL                                         = MK_CNTRY( 'S', 'N', 0 ),             /* SN Senegal */
    WIFI_COUNTRY_SERBIA                                          = MK_CNTRY( 'R', 'S', 0 ),             /* RS Serbia */
    WIFI_COUNTRY_SEYCHELLES                                      = MK_CNTRY( 'S', 'C', 0 ),             /* SC Seychelles */
    WIFI_COUNTRY_SIERRA_LEONE                                    = MK_CNTRY( 'S', 'L', 0 ),             /* SL Sierra_Leone */
    WIFI_COUNTRY_SINGAPORE                                       = MK_CNTRY( 'S', 'G', 0 ),             /* SG Singapore */
    WIFI_COUNTRY_SLOVAKIA                                        = MK_CNTRY( 'S', 'K', 0 ),             /* SK Slovakia */
    WIFI_COUNTRY_SLOVENIA                                        = MK_CNTRY( 'S', 'I', 0 ),             /* SI Slovenia */
    WIFI_COUNTRY_SOLOMON_ISLANDS                                 = MK_CNTRY( 'S', 'B', 0 ),             /* SB Solomon_Islands */
    WIFI_COUNTRY_SOMALIA                                         = MK_CNTRY( 'S', 'O', 0 ),             /* SO Somalia */
    WIFI_COUNTRY_SOUTH_AFRICA                                    = MK_CNTRY( 'Z', 'A', 0 ),             /* ZA South_Africa */
    WIFI_COUNTRY_SPAIN                                           = MK_CNTRY( 'E', 'S', 0 ),             /* ES Spain */
    WIFI_COUNTRY_SRI_LANKA                                       = MK_CNTRY( 'L', 'K', 0 ),             /* LK Sri_Lanka */
    WIFI_COUNTRY_SURINAME                                        = MK_CNTRY( 'S', 'R', 0 ),             /* SR Suriname */
    WIFI_COUNTRY_SWAZILAND                                       = MK_CNTRY( 'S', 'Z', 0 ),             /* SZ Swaziland */
    WIFI_COUNTRY_SWEDEN                                          = MK_CNTRY( 'S', 'E', 0 ),             /* SE Sweden */
    WIFI_COUNTRY_SWITZERLAND                                     = MK_CNTRY( 'C', 'H', 0 ),             /* CH Switzerland */
    WIFI_COUNTRY_SYRIAN_ARAB_REPUBLIC                            = MK_CNTRY( 'S', 'Y', 0 ),             /* SY Syrian_Arab_Republic */
    WIFI_COUNTRY_TAIWAN_PROVINCE_OF_CHINA                        = MK_CNTRY( 'T', 'W', 0 ),             /* TW Taiwan,_Province_Of_China */
    WIFI_COUNTRY_TAJIKISTAN                                      = MK_CNTRY( 'T', 'J', 0 ),             /* TJ Tajikistan */
    WIFI_COUNTRY_TANZANIA_UNITED_REPUBLIC_OF                     = MK_CNTRY( 'T', 'Z', 0 ),             /* TZ Tanzania,_United_Republic_Of */
    WIFI_COUNTRY_THAILAND                                        = MK_CNTRY( 'T', 'H', 0 ),             /* TH Thailand */
    WIFI_COUNTRY_TOGO                                            = MK_CNTRY( 'T', 'G', 0 ),             /* TG Togo */
    WIFI_COUNTRY_TONGA                                           = MK_CNTRY( 'T', 'O', 0 ),             /* TO Tonga */
    WIFI_COUNTRY_TRINIDAD_AND_TOBAGO                             = MK_CNTRY( 'T', 'T', 0 ),             /* TT Trinidad_and_Tobago */
    WIFI_COUNTRY_TUNISIA                                         = MK_CNTRY( 'T', 'N', 0 ),             /* TN Tunisia */
    WIFI_COUNTRY_TURKEY                                          = MK_CNTRY( 'T', 'R', 0 ),             /* TR Turkey */
    WIFI_COUNTRY_TURKMENISTAN                                    = MK_CNTRY( 'T', 'M', 0 ),             /* TM Turkmenistan */
    WIFI_COUNTRY_TURKS_AND_CAICOS_ISLANDS                        = MK_CNTRY( 'T', 'C', 0 ),             /* TC Turks_and_Caicos_Islands */
    WIFI_COUNTRY_TUVALU                                          = MK_CNTRY( 'T', 'V', 0 ),             /* TV Tuvalu */
    WIFI_COUNTRY_UGANDA                                          = MK_CNTRY( 'U', 'G', 0 ),             /* UG Uganda */
    WIFI_COUNTRY_UKRAINE                                         = MK_CNTRY( 'U', 'A', 0 ),             /* UA Ukraine */
    WIFI_COUNTRY_UNITED_ARAB_EMIRATES                            = MK_CNTRY( 'A', 'E', 0 ),             /* AE United_Arab_Emirates */
    WIFI_COUNTRY_UNITED_KINGDOM                                  = MK_CNTRY( 'G', 'B', 0 ),             /* GB United_Kingdom */
    WIFI_COUNTRY_UNITED_STATES                                   = MK_CNTRY( 'U', 'S', 0 ),             /* US United_States */
    WIFI_COUNTRY_UNITED_STATES_REV4                              = MK_CNTRY( 'U', 'S', 4 ),             /* US United_States Revision 4 */
    WIFI_COUNTRY_UNITED_STATES_NO_DFS                            = MK_CNTRY( 'Q', '2', 0 ),             /* Q2 United_States_(No_DFS) */
    WIFI_COUNTRY_UNITED_STATES_MINOR_OUTLYING_ISLANDS            = MK_CNTRY( 'U', 'M', 0 ),             /* UM United_States_Minor_Outlying_Islands */
    WIFI_COUNTRY_URUGUAY                                         = MK_CNTRY( 'U', 'Y', 0 ),             /* UY Uruguay */
    WIFI_COUNTRY_UZBEKISTAN                                      = MK_CNTRY( 'U', 'Z', 0 ),             /* UZ Uzbekistan */
    WIFI_COUNTRY_VANUATU                                         = MK_CNTRY( 'V', 'U', 0 ),             /* VU Vanuatu */
    WIFI_COUNTRY_VENEZUELA                                       = MK_CNTRY( 'V', 'E', 0 ),             /* VE Venezuela */
    WIFI_COUNTRY_VIET_NAM                                        = MK_CNTRY( 'V', 'N', 0 ),             /* VN Viet_Nam */
    WIFI_COUNTRY_VIRGIN_ISLANDS_BRITISH                          = MK_CNTRY( 'V', 'G', 0 ),             /* VG Virgin_Islands,_British */
    WIFI_COUNTRY_VIRGIN_ISLANDS_US                               = MK_CNTRY( 'V', 'I', 0 ),             /* VI Virgin_Islands,_U.S. */
    WIFI_COUNTRY_WALLIS_AND_FUTUNA                               = MK_CNTRY( 'W', 'F', 0 ),             /* WF Wallis_and_Futuna */
    WIFI_COUNTRY_WEST_BANK                                       = MK_CNTRY( '0', 'C', 0 ),             /* 0C West_Bank */
    WIFI_COUNTRY_WESTERN_SAHARA                                  = MK_CNTRY( 'E', 'H', 0 ),             /* EH Western_Sahara */
    WIFI_COUNTRY_WORLD_WIDE_XX                                   = MK_CNTRY( 'X', 'X', 0 ),             /* Worldwide Locale (passive Ch12-14) */
    WIFI_COUNTRY_YEMEN                                           = MK_CNTRY( 'Y', 'E', 0 ),             /* YE Yemen */
    WIFI_COUNTRY_ZAMBIA                                          = MK_CNTRY( 'Z', 'M', 0 ),             /* ZM Zambia */
    WIFI_COUNTRY_ZIMBABWE                                        = MK_CNTRY( 'Z', 'W', 0 ),             /* ZW Zimbabwe */
} wifi_country_code_t;

#ifdef __cplusplus
 }
#endif

#endif /* _ADAFRUIT_CONSTANTS_H_ */
