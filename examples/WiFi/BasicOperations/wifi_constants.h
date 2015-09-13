/*
 * Copyright 2014, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *  Defines common constants used with WICED
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
    WICED_SECURITY_OPEN           = 0,                                                /**< Open security                           */
    WICED_SECURITY_WEP_PSK        = WEP_ENABLED,                                      /**< WEP Security with open authentication   */
    WICED_SECURITY_WEP_SHARED     = ( WEP_ENABLED | SHARED_ENABLED ),                 /**< WEP Security with shared authentication */
    WICED_SECURITY_WPA_TKIP_PSK   = ( WPA_SECURITY  | TKIP_ENABLED ),                 /**< WPA Security with TKIP                  */
    WICED_SECURITY_WPA_AES_PSK    = ( WPA_SECURITY  | AES_ENABLED ),                  /**< WPA Security with AES                   */
    WICED_SECURITY_WPA2_AES_PSK   = ( WPA2_SECURITY | AES_ENABLED ),                  /**< WPA2 Security with AES                  */
    WICED_SECURITY_WPA2_TKIP_PSK  = ( WPA2_SECURITY | TKIP_ENABLED ),                 /**< WPA2 Security with TKIP                 */
    WICED_SECURITY_WPA2_MIXED_PSK = ( WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED ),   /**< WPA2 Security with AES & TKIP           */

    WICED_SECURITY_WPS_OPEN       = ( WPS_ENABLED ),                                  /**< WPS with open security                  */
    WICED_SECURITY_WPS_SECURE     = ( WPS_ENABLED | AES_ENABLED),                     /**< WPS with AES security                   */

    WICED_SECURITY_UNKNOWN        = -1,                                               /**< May be returned by scan function if security is unknown. Do not pass this to the join function! */

    WICED_SECURITY_FORCE_32_BIT   = 0x7fffffff                                        /**< Exists only to force wiced_security_t type to 32 bits */
} wiced_security_t;

/**
 * Enumeration of network types
 */
typedef enum
{
    WICED_BSS_TYPE_INFRASTRUCTURE = 0, /**< Denotes infrastructure network                  */
    WICED_BSS_TYPE_ADHOC          = 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
    WICED_BSS_TYPE_ANY            = 2, /**< Denotes either infrastructure or ad-hoc network */

    WICED_BSS_TYPE_UNKNOWN        = -1 /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
} wiced_bss_type_t;

/**
 * Enumeration of 802.11 radio bands
 */
typedef enum
{
    WICED_802_11_BAND_5GHZ   = 0, /**< Denotes 5GHz radio band   */
    WICED_802_11_BAND_2_4GHZ = 1  /**< Denotes 2.4GHz radio band */
} wiced_802_11_band_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef _WIFI_CONSTANTS_H_ */
