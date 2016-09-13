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

/*  This example use the default RootCA list to access secured sites:
 *  1. Change ssid/pass to match your network
 *  2. Choose the SERVER_ID or use your own server e.g www.adafruit.com
 *  3. Compile and run the sketch
*/

#include <adafruit_feather.h>
#include <adafruit_http.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPass"

#define HTTPS_PORT            443

#define ADAFRUIT_S3_SERVER    "adafruit-download.s3.amazonaws.com"

// Some servers such as Facebook check the user_agent header to
// return data accordingly. Setting 'curl' mimics a command line browser.
// For a list of popular user agents see:
//  http://www.useragentstring.com/pages/useragentstring.php
#define USER_AGENT_HEADER    "curl/7.45.0"

// Change the SERVER_ID to pick the website to test with
#define SERVER_ID    0

const char * server_arr[][2] =
{
    [0] = { "www.adafruit.com"       , "/" },
    [1] = { "twitter.com"            , "/" },
    [2] = { "www.google.com"         , "/" },
    [3] = { "www.facebook.com"       , "/" },
    [4] = { "aws.amazon.com"         , "/" },
    [5] = { "github.com"             , "/" },
};

// You can set your own server & url here
const char * server = server_arr[SERVER_ID][0];
const char * url    = server_arr[SERVER_ID][1];

int ledPin = PA15;

// Use the HTTP class
AdafruitHTTP http;

/**************************************************************************/
/*!
    @brief  TCP/HTTP received callback
*/
/**************************************************************************/
void receive_callback(void)
{
  // if there are incoming bytes available
  // from the server, read then print them:
  while ( http.available() )
  {
    int c = http.read();
    Serial.write( (isprint(c) || iscntrl(c)) ? ((char)c) : '.');
  }
}

/**************************************************************************/
/*!
    @brief  TCP/HTTP disconnect callback
*/
/**************************************************************************/
void disconnect_callback(void)
{
  Serial.println();
  Serial.println("---------------------");
  Serial.println("DISCONNECTED CALLBACK");
  Serial.println("---------------------");
  Serial.println();

  http.stop();
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("HTTPS Default Root CA Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the HTTP client to auto print error codes and halt on errors
  http.err_actions(true, true);

  // Set the HTTP client timeout (in ms)
  http.setTimeout(1000);

  // Set the callback handlers
  http.setReceivedCallback(receive_callback);
  http.setDisconnectCallback(disconnect_callback);

  Serial.printf("Connecting to %s port %d ... ", server, HTTPS_PORT );
  http.connectSSL(server, HTTPS_PORT); // Will halt if an error occurs
  Serial.println("OK");

  // Make a HTTP request:
  http.addHeader("User-Agent", USER_AGENT_HEADER);
  http.addHeader("Accept", "text/html");
  http.addHeader("Connection", "keep-alive");

  Serial.printf("Requesting '%s' ... ", url);
  http.get(server, url); // Will halt if an error occurs
  Serial.println("OK");
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  togglePin(ledPin);
  delay(250);
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
