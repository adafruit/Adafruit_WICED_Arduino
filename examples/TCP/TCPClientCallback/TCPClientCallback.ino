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

/* This example will send an HTTP request to the indicated HTTP
 * server and page. The response will be printed via the 'data received'
 * callback handler.
 */

#include <adafruit_feather.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

#define SERVER               "wifitest.adafruit.com"     // The TCP server to connect to
#define PAGE                 "/testwifi/index.html" // The HTTP resource to request
#define PORT                 80                     // The TCP port to use

AdafruitTCP tcp;

/**************************************************************************/
/*!
    @brief  TCP received callback
*/
/**************************************************************************/
void receive_callback(void)
{
  // if there are incoming bytes available
  // from the server, read then print them:
  while ( tcp.available() )
  {
    int c = tcp.read();
    Serial.write( (isprint(c) || iscntrl(c)) ? ((char)c) : '.');
  }
}

/**************************************************************************/
/*!
    @brief  TCP disconnect callback
*/
/**************************************************************************/
void disconnect_callback(void)
{
  Serial.println();
  Serial.println("---------------------");
  Serial.println("DISCONNECTED CALLBACK");
  Serial.println("---------------------");
  Serial.println();

  tcp.stop();
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the serial port to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("TCP Client Example (Callbacks)\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the TCP client to auto print error codes and halt on errors
  tcp.err_actions(true, true);

  // Install callbacks
  tcp.setReceivedCallback(receive_callback);
  tcp.setDisconnectCallback(disconnect_callback);

  // Start connection
  Serial.printf("Connecting to %s port %d ... ", SERVER, PORT);
  tcp.connect(SERVER, PORT); // Will halt if an error occurs
  Serial.println("OK");

  // Make an HTTP request:
  tcp.printf("GET %s HTTP/1.1\r\n", PAGE);
  tcp.print("Host: "); tcp.println(SERVER);
  tcp.println("Connection: close");
  tcp.println();

  // Data is buffered and will only be sent when the network packet is full
  // or flush() is called to optimize network usage
  // tcp.flush();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
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
