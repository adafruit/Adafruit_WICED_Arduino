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

/* This example demonstrates how to use the setNtpServer function
 * to sync the time with the network once a day. setNtpServer can take 
 * hostname or a specific IP address. If setNtpServer(NULL) is invoked, 
 * syncing with NTP Server is disabled.
 * 
 * Note: Upon connected, WICED will try to get time from default NTP Server 
 * which is pool.ntp.org server. To disable this, you need to call 
 * setNtpServer(NULL) before connecting.
 */

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

// target by hostname
const char ntp_server[] = "pool.ntp.org";

iso8601_time_t iso_time;

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the Serial Monitor to open
  while (!Serial)
  {
    /* Delay required to avoid RTOS task switching problems */
    delay(1);
  }

  Serial.println("Set NTP Server Example\r\n");

  // Print all software versions
  Feather.printVersions();

  // Disable auto NTP server upon connection
  Feather.setNtpServer(NULL);

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Print the time before set up NTP
  Serial.println("Time before setup NTP server");
  Feather.getISO8601Time(&iso_time);
  Serial.println( (char*) &iso_time);

  // Configure the desired NTP Server
  Serial.println();
  Serial.println("Set up NTP Server");
  Feather.setNtpServer(ntp_server);
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
    Feather.getISO8601Time(&iso_time);
    Serial.println( (char*) &iso_time);
    
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
