/** @file
 *  Defines common constants used with feather board
 *
 */

#ifndef _WIFI_CONSTANTS_H_
#define _WIFI_CONSTANTS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define SHARED_ENABLED  0x00008000
#define WPA_SECURITY    0x00200000
#define WPA2_SECURITY   0x00400000
#define WPS_ENABLED     0x10000000

#define SHARED_AUTH     0x0001
#define WEP_ENABLED     0x0001
#define TKIP_ENABLED    0x0002
#define AES_ENABLED     0x0004

/**
 * Enumeration of Wi-Fi security modes
 */
typedef enum
{
    SECURITY_OPEN           = 0,                                                /**< Open security                           */
    SECURITY_WEP_PSK        = WEP_ENABLED,                                      /**< WEP Security with open authentication   */
    SECURITY_WEP_SHARED     = ( WEP_ENABLED | SHARED_ENABLED ),                 /**< WEP Security with shared authentication */
    SECURITY_WPA_TKIP_PSK   = ( WPA_SECURITY  | TKIP_ENABLED ),                 /**< WPA Security with TKIP                  */
    SECURITY_WPA_AES_PSK    = ( WPA_SECURITY  | AES_ENABLED ),                  /**< WPA Security with AES                   */
    SECURITY_WPA2_AES_PSK   = ( WPA2_SECURITY | AES_ENABLED ),                  /**< WPA2 Security with AES                  */
    SECURITY_WPA2_TKIP_PSK  = ( WPA2_SECURITY | TKIP_ENABLED ),                 /**< WPA2 Security with TKIP                 */
    SECURITY_WPA2_MIXED_PSK = ( WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED ),   /**< WPA2 Security with AES & TKIP           */

    SECURITY_WPS_OPEN       = ( WPS_ENABLED ),                                  /**< WPS with open security                  */
    SECURITY_WPS_SECURE     = ( WPS_ENABLED | AES_ENABLED),                     /**< WPS with AES security                   */

    SECURITY_UNKNOWN        = -1,                                               /**< May be returned by scan function if security is unknown. Do not pass this to the join function! */

    SECURITY_FORCE_32_BIT   = 0x7fffffff                                        /**< Exists only to force security_t type to 32 bits */
} security_t;

/**
 * Enumeration of network types
 */
typedef enum
{
    BSS_TYPE_INFRASTRUCTURE = 0, /**< Denotes infrastructure network                  */
    BSS_TYPE_ADHOC          = 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
    BSS_TYPE_ANY            = 2, /**< Denotes either infrastructure or ad-hoc network */

    BSS_TYPE_UNKNOWN        = -1 /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
} bss_type_t;

/**
 * Enumeration of 802.11 radio bands
 */
typedef enum
{
    BAND_802_11_5GHZ   = 0, /**< Denotes 5GHz radio band   */
    BAND_802_11_2_4GHZ = 1  /**< Denotes 2.4GHz radio band */
} band_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef _WIFI_CONSTANTS_H_ */
