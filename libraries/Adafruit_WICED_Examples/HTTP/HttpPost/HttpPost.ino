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

/* This example posts data to the defined URL */

#include <adafruit_feather.h>
#include <adafruit_http.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

#define SERVER               "wifitest.adafruit.com"     // The TCP server to connect to
#define PAGE                 "/testwifi/testpost.php" // The HTTP resource to POST
#define PORT                 80                       // The TCP port to use

// TODO help user to encoded post data
const char* post_data[][2] =
{
    {"name"  , "feather"},
    {"email" , "feather@adafruit.com"}
};

// Some servers such as Facebook check the user_agent header to
// return data accordingly. Setting 'curl' mimics a command line browser.
// For a list of popular user agents see: http://www.useragentstring.com/pages/useragentstring.php
#define USER_AGENT_HEADER    "curl/7.45.0"

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
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the USB serial to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("HTTP POST Example\r\n");

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

  // Set HTTP client verbose
  http.verbose(true);

  // Set the callback handlers
  http.setReceivedCallback(receive_callback);

  // Connect to the HTTP server
  Serial.printf("Connecting to %s port %d ... ", SERVER, PORT);
  http.connect(SERVER, PORT); // Will halt if an error occurs
  Serial.println("OK");

  // Setup the HTTP request with any required header entries
  http.addHeader("User-Agent", USER_AGENT_HEADER);
  http.addHeader("Connection", "keep-alive");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send the HTTP POST
  Serial.printf("Posting to '%s' ... ", PAGE);
  http.post(PAGE, post_data, 2); // Will halt if an error occurs
  Serial.println("OK");
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  // If there are incoming bytes available
  // from the server, read then print them:
  while (http.available())
  {
    int c = http.read();
    Serial.write( (isprint(c) || iscntrl(c)) ? ((char)c) : '.');
  }

  // If the server is disconnected, stop the client:
  if ( !http.connected() )
  {
    Serial.println();
    Serial.println("Disconnecting from server.");
    http.stop();

    // Hang around here forever!
    while (true) delay(1);
  }
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
