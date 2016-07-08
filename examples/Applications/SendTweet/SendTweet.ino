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
#include "adafruit_http.h"
#include "adafruit_sha1.h"
#include "adafruit_twitter.h"

/* This example demonstrates how to use AdafrtuiTwitter class
 * to send out a tweet
 *
 * To run this demo:
 * 1. Goto https://apps.twitter.com/ and login
 * 2. Create an application to use with this WICED Feather
 * 3. (Optional) You could change the access level to give the applicaion
 * permission to send DM. It is advised to do so, do that you could use WICED
 * to send DM in other example
 * 4. In the app management click "manage keys and access tokens"
 * and then click "Create my access token"
 * 5. Change CONSUMER_KEY, CONSUMER_SECRET, TOKEN_ACCESS, TOKEN_SECRET accordingly
 * 6. Change your TWEET status
 * 7. Compile and run, if you run this sketch too often, Twitter server may reject
 * your connection request, just wait a few minutes and try again.
 */

// Network
#define WLAN_SSID               "yourSSID"
#define WLAN_PASS               "yourPassword"

// Twitter Account
#define CONSUMER_KEY            "YOUR_CONSUMER_KEY"
#define CONSUMER_SECRET         "YOUR_CONSUMER_SECRET"

#define TOKEN_ACCESS            "YOUR_TOKEN_ACCESS"
#define TOKEN_SECRET            "YOUR_TOKEN_SECRET"

#define TWEET                   "Hello from Adafruit WICED Feather"

AdafruitTwitter Twitter;

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

  Serial.println("Twitter Send Tweet Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  Twitter.begin(CONSUMER_KEY, CONSUMER_SECRET, TOKEN_ACCESS, TOKEN_SECRET);
  Twitter.err_actions(true, true);

  Serial.print("Sending tweet: " TWEET " ... ");
  Twitter.tweet(TWEET);
  Serial.println("OK");

  Twitter.stop();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  togglePin(PA15);
  delay(1000);
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
