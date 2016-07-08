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

#include "adafruit_feather.h"

/* This example demonstrates how to use config() to configure static IP.
 * Example also try to resolve a hostname to make sure it works. 
 */

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

IPAddress staticIP     (192, 168, 0, 145);
IPAddress staticGateway(192, 168, 0, 1);
IPAddress staticNetmask(255, 255, 255, 0);

// target by hostname
const char hostname[] = "adafruit.com";

void disconnect_callback(void)
{
  Serial.println("disconnect_callback(): Disconnected");
}

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

  Serial.println("Static IP Example\r\n");

  // Print all software versions
  Feather.printVersions();

  // Set disconnection callback
  Feather.setDisconnectCallback(disconnect_callback);

  // Config static IP
  Serial.println("Configuring Static IP");
  Feather.config(staticIP, staticGateway, staticNetmask);

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  if ( Feather.connected() )
  {
    // Resolve and ping hostname
    IPAddress ipaddr;

    ipaddr = Feather.hostByName(hostname);
    Serial.print(hostname);
    Serial.print(" -> ");
    Serial.println(ipaddr);
    
    Serial.println();
    Serial.println("Try again in 10 seconds");
    Serial.println();
    delay(10000);
  }
}

/**************************************************************************/
/*!
    @brief  Connect to defined Access Point
*/
/**************************************************************************/
bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Please wait while connecting to: '" WLAN_SSID "' ... ");

  if ( Feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errno());
    Serial.println();
  }
  Serial.println();

  return Feather.connected();
}
