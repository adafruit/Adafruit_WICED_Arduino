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

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

// target by hostname
const char target_hostname[] = "adafruit.com";

// target by IP String
const char target_ip_str[] = "8.8.8.8";

void disconnect_callback(void)
{
  Serial.println("disconnect_callback(): Disconnected");
}

bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);
  
  if ( feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.printf("Failed! %s (%d)", feather.errstr(), feather.errno());
    Serial.println();
  }
  Serial.println("");
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);
  
  // wait for Serial
  while (!Serial) delay(1);

  Serial.println("GetHostByName Example\r\n");

  // Set disconnection callback
  feather.setDisconnectCallback(disconnect_callback);

  connectAP();
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
    // Resolve and ping hostname
    IPAddress ipaddr;
   
    ipaddr = feather.hostByName(target_hostname);
    Serial.print(target_hostname);
    Serial.print(": ");
    Serial.println(ipaddr);
    
    ipaddr = feather.hostByName(target_ip_str);
    Serial.print(target_ip_str);
    Serial.print(": ");
    Serial.println(ipaddr);

    Serial.println();
    Serial.println("Try again in 10 seconds");
    Serial.println();
    delay(10000);
  }
}
