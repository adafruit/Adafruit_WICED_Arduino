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

#include "adafruit_wifi.h"

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

// Ping target by hostname
const char target_hostname[] = "adafruit.com";

// Ping target by IP String
const char target_ip_str[] = "8.8.8.8";

// Ping target by IPAddress object
IPAddress target_ip(8, 8, 4, 4);

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  // wait for Serial
  while (!Serial) delay(1);

  Serial.println(F("Ping Example\r\n"));

  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);

  if ( feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println(F("Connected!"));
  }
  else
  {
    Serial.print(F("Failed! Error: 0x"));
    Serial.println(feather.errno(), HEX);
  }
  Serial.println("");
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{ 
  if ( feather.connected() )
  {
    // Resolve hostname
    IPAddress ip = feather.hostByName(target_hostname);
    Serial.print(target_hostname);
    Serial.print(": ");
    Serial.println(ip);
    
    
  }

  Serial.println(F("\r\n"));
  delay(10000);
}
