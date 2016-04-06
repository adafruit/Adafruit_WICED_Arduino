/*********************************************************************
 This is an example for our WICED Feather WIFI modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This example use the AdafruitHTTP class to send requests and get
 * the response via a callback */

#include <adafruit_feather.h>
#include <adafruit_http.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

#define SERVER               "www.adafruit.com"     // The TCP server to connect to
#define PAGE                 "/testwifi/index.html" // The HTTP resource to request
#define PORT                 80                     // The TCP port to use

// Some servers such as Facebook check the user_agent header to
// return data accordingly. Setting 'curl' mimics a command line browser.
// For a list of popular user agents see: http://www.useragentstring.com/pages/useragentstring.php
#define USER_AGENT_HEADER    "curl/7.45.0"

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
  // If there are incoming bytes available
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
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the USB serial to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("HTTP Get Example (Callback Based)\r\n");

  // Print all software versions
  Feather.printVersions();

  // Try to connect to an AP
  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the HTTP client to auto print error codes and halt on errors
  http.err_actions(true, true);

  // Set the callback handlers
  http.setReceivedCallback(receive_callback);
  http.setDisconnectCallback(disconnect_callback);

  // Connect to the HTTP server
  Serial.printf("Connecting to %s port %d ... ", SERVER, PORT);
  http.connect(SERVER, PORT); // Will halt if an error occurs
  Serial.println("OK");

  // Setup the HTTP request with any required header entries
  http.addHeader("User-Agent", USER_AGENT_HEADER);
  http.addHeader("Accept", "text/html");
  http.addHeader("Connection", "keep-alive");

  // Send the HTTP request
  Serial.printf("Requesting '%s' ... ", PAGE);
  http.get(PAGE); // Will halt if an error occurs
  Serial.println("OK");
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  togglePin(ledPin);
  delay(250);
}

/**************************************************************************/
/*!
    @brief  Connect to the defined access point (AP)
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
