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
  // wait for Serial
  while (!Serial) delay(1);

  Serial.println(F("Ping Example\r\n"));

  // Set disconnection callback
  feather.setDisconnectCallback(disconnect_callback);
  
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
    uint8_t resp_ms;

    // Ping hostname
    Serial.printf("Pinging %s : ", target_hostname);
    resp_ms = feather.ping(target_hostname);
    if (resp_ms != 0)
    {
      Serial.printf("time = %d ms", resp_ms);
    }else
    {
      Serial.printf("error = 0x%04X", feather.errno());
    }
    Serial.println();

    // Ping IP Address in String format
    Serial.printf("Pinging %s : ", target_ip_str);
    resp_ms = feather.ping(target_ip_str);
    if (resp_ms != 0)
    {
      Serial.printf("time = %d ms", resp_ms);
    }else
    {
      Serial.printf("error = 0x%04X", feather.errno());
    }
    Serial.println();

    // Ping specific IP Address
    Serial.printf("Pinging %s : ", target_ip.toCharArray());
    resp_ms = feather.ping(target_ip);
    if (resp_ms != 0)
    {
      Serial.printf("time = %d ms", resp_ms);
    }else
    {
      Serial.printf("error = 0x%04X", feather.errno());
    }
    Serial.println();

    Serial.println();
    Serial.println("Try again in 10 seconds");
    Serial.println();
    delay(10000);
  }
}
