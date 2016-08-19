/*********************************************************************
 This is an example for our Feather WIFI modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This example scans for nearby networks and prints out the SSID
 * details every 10 seconds. Hidden SSIDs are printed as '*'.
 */

#include <adafruit_feather.h>
#include <adafruit_featherap.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"
#define WLAN_ENCRYPTION       ENC_TYPE_WPA2_AES
#define WLAN_CHANNEL          1

IPAddress apIP     (192, 168, 2, 1);
IPAddress apGateway(192, 168, 2, 1);
IPAddress apNetmask(255, 255, 255, 0);

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("SoftAP Client RSSI Example\r\n");

  FeatherAP.begin(apIP, apGateway, apNetmask, WLAN_CHANNEL);
  FeatherAP.start(WLAN_SSID, WLAN_PASS, WLAN_ENCRYPTION);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  Serial.println();
  Serial.println("Scanning available networks...");

  Serial.println();
  Serial.println("Waiting 10 seconds before trying again");
  delay(10000);
}
