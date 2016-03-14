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

#include <adafruit_feather.h>
#include <adafruit_mqtt.h>
#include <adafruit_aio.h>

/* This sketch connects to the Adafruit IO server at io.adafruit.com.
 * It updates a 'PHOTOCELL_FEED' every 5 seconds and follows state change
 * from 'ONOFF_FEED' using callback
 *
 * To run this demo
 * 1. Change WLAN_SSID/WLAN_PASS
 * 2. Decide whether you want to use TLS/SSL or not (USE_TLS)
 * 3. Change AIO_USERNAME, AIO_KEY to match your own account details
 * 4. If you want, change PHOTOCELL_FEED and ONOFF_FEED to use different feeds
 * 5. Compile and run
 * 6. Optionally log into the AIO webserver to see any changes in data, etc.
 */

#define WLAN_SSID         "yourSSID"
#define WLAN_PASS         "yourPass"

#define AIO_USERNAME      "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_KEY           "...your AIO key..."

// AdafruitAIO will auto append the "username/feeds/" prefix to your feed(s)
#define PHOTOCELL_FEED     "photocell"
#define ONOFF_FEED         "onoff"

// Connect using TLS/SSL or not
#define USE_TLS             0

// Uncomment to set your own ClientID, otherwise a random ClientID is used
//#define CLIENTID          "Adafruit Feather"

AdafruitAIO       aio(AIO_USERNAME, AIO_KEY);
AdafruitAIOFeed   photocell (&aio, PHOTOCELL_FEED);
AdafruitAIOFeed   onoff     (&aio, ONOFF_FEED);

int value = 0;

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("AIO Test Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the MQTT client to auto print error codes and halt on errors
  aio.err_actions(true, true);

  // Set ClientID if defined
  #ifdef CLIENTID
  aio.clientID(CLIENTID);
  #endif

  Serial.print("Connecting to io.adafruit.com ... ");
  if ( USE_TLS )
  {
    aio.connectSSL(); // Will halted if an error occurs
  }else
  {
    aio.connect(); // Will halted if an error occurs
  }
  Serial.println("OK");

  // 'Follow' the onoff feed to capture any state changes
  onoff.follow(feed_callback);
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  value = (value+1) % 100;

  Serial.print("Updating feed " PHOTOCELL_FEED " : ");
  Serial.print(value);
  photocell.print(value);
  Serial.println(" ... OK");

  delay(5000);
}

/**************************************************************************/
/*!
    @brief  'follow' event callback handler

    @param  message    The new value associated with this feed

    @note   'message' is a UTF8String (byte array), which means
            it is not null-terminated like C-style strings. You can
            access its data and len using .data & .len, although there is
            also a Serial.print override to handle UTF8String data types.
*/
/**************************************************************************/
void feed_callback(UTF8String message)
{
  // Print message
  Serial.print("[ONOFF Feed] : ");
  Serial.println(message);
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
