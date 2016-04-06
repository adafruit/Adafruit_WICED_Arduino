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
 * and follow `COLOR_FEED' changes. User can also enter new RGB
 * value via Serial to update the feed
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
#define COLOR_FEED         "color"

// Connect using TLS/SSL or not
#define USE_TLS             0

// Uncomment to set your own ClientID, otherwise a random ClientID is used
//#define CLIENTID          "Adafruit Feather"

AdafruitAIO           aio(AIO_USERNAME, AIO_KEY);
AdafruitAIOFeedColor  color(&aio, COLOR_FEED, MQTT_QOS_AT_LEAST_ONCE);

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

  Serial.println("AIO Feed Color Example\r\n");

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

  // follow text feed
  Serial.print("Subcribing to feed: '" COLOR_FEED "' ... ");
  color.follow(color_callback);
  Serial.println("OK");
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  // Get input from user '0' to off, '1' to update feed
  if ( Serial.available() )
  {
    // Get input and echo
    char* input = getUserInput();
    Serial.println(input);

    // Parse the input
    uint8_t rgb[3];
    int count = sscanf(input, "%x, %x, %x", &rgb[0], &rgb[1], &rgb[2]);

    if ( count != 3 ) 
    {
      Serial.println("Invalid input, please enter comma separated HEX for RGB (wihtout 0x prefix)");
      Serial.println("Example: 'ff, 00, aa'");

      Serial.println();
      Serial.print("Enter RGB (hex) with comma separated (e.g ff,00,bb) : ");
      return;
    }
    
    // AIO Feed OnOff can be update with assignment like normal string variable
    color = rgb;
  }
}

/**************************************************************************/
/*!
    @brief  Feed Text callback event

    @param  str      current text of the feed (true or false)
*/
/**************************************************************************/
void color_callback(uint8_t rgb[3])
{
  Serial.printf("Feed value: RGB = (%02X, %02X, %02X)", rgb[0], rgb[1], rgb[2]);
  Serial.println();

  // print prompt
  Serial.print("Enter RGB (hex) with comma separated (e.g ff, 00, bb) : ");
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

/**************************************************************************/
/*!
    @brief  Get user input from Serial
*/
/**************************************************************************/
char* getUserInput(void)
{
  static char inputs[64+1];
  memset(inputs, 0, sizeof(inputs));

  // wait until data is available
  while( Serial.available() == 0 ) { delay(1); }

  uint8_t count=0;
  do
  {
    count += Serial.readBytes(inputs+count, 64);
  } while( (count < 64) && Serial.available() );

  return inputs;
}

