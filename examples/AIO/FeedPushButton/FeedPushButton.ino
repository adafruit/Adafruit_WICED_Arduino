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

/* This sketch connect to the Adafruit IO sever io.adafruit.com
 * and follow `PUSHBUTTON_FEED' changes. User can also enter new value via
 * Serial to update the feed. LED is update to reflect the current state of
 * the feed.
 *
 * To run this demo
 * 1. Change SSID/PASS
 * 2. Decide whether you want to use TLS/SSL or not (USE_TLS)
 * 3. Enable CLIENTID if you want
 * 4. Compile and run
 * 5. Log into the AIO webserver to make/see any changes in data, etc.
 */

#define WLAN_SSID         "yourSSID"
#define WLAN_PASS         "yourPass"

#define AIO_USERNAME      "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_KEY           "...your AIO key..."

// AdafruitAIO will auto append "username/feeds/" prefix to your feed"
#define PUSHBUTTON_FEED    "pushbutton"

// Connect using TLS/SSL or not
#define USE_TLS             0

// Uncomment to set your own ClientID, otherwise a random ClientID is used
//#define CLIENTID          "Adafruit Feather"

AdafruitAIO                 aio(AIO_USERNAME, AIO_KEY);
AdafruitAIOFeedPushButton   pushbutton(&aio, PUSHBUTTON_FEED);

int ledPin = PA15;

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("AIO Feed Push Button Example\r\n");

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

  // follow onoff state change
  Serial.print("Subcribing to feed: '" PUSHBUTTON_FEED "' ... ");
  pushbutton.follow(push_callback);
  Serial.println("OK");

  // Message to user
  Serial.print("Enter '0' or '1' to update feed: ");
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  // Get input from user '0' to off, '1' to on LED and feed
  // AIO Feed OnOff can be assigned like normal boolean
  // It will auto update feed value with correct format
  if ( Serial.available() )
  {
    char c = Serial.read();

    // echo
    Serial.println(c);
    Serial.print("Enter '0' or '1' to update feed: ");

    // No need to update LED here, feed update will make AIO server
    // send message to our module and LED will be update in the callback
    if ( c == '0' )
    {
      pushbutton = false;
    }else if ( c == '1' )
    {
      pushbutton = true;
    }else
    {
      // do nothing
    }
  }
}

/**************************************************************************/
/*!
    @brief  Feed OnOff callback event

    @param  state      new state of the feed (true or false)
*/
/**************************************************************************/
void push_callback(bool state)
{
  digitalWrite(ledPin, state);
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

