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

/* This example connect to Adafruit's large text file (10KB, 100KB, 1MB)
 * to test the large data handling. In addition to print the contents,
 * the sketch also compute checksum of the content to compare with one
 * calculated by other mean.
 *
 * To run the sketch
 * 1. Change SSID/Pass
 * 2. Choose the file to download by change FILE_ID
 * 3. Combine and run the sketch
 * */

#include <adafruit_feather.h>
#include <adafruit_http.h>

#define WLAN_SSID             "yourSSID"
#define WLAN_PASS             "yourPass"

#define SERVER                "adafruit-download.s3.amazonaws.com"
#define HTTPS_PORT            443

int ledPin = PA15;

// Change the SERVER_ID to match the generated certificates.h
#define FILE_ID    2

// S3 server to test large files,
const char * file_arr[] =
{
    [0] = "/text_10KB.txt"  ,
    [1] = "/text_100KB.txt" ,
    [2] = "/text_1MB.txt"   ,
};

const char * url = file_arr[FILE_ID];

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

  Serial.print("Total byte read: ");
  Serial.println( http.byteRead() );

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

  Serial.println("HTTPS Large Data Example\r\n");

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

  // Set up callbacks
  http.setReceivedCallback(receive_callback);
  http.setDisconnectCallback(disconnect_callback);

  // Start a secure connection
  Serial.printf("Connecting to %s port %d ... ", SERVER, HTTPS_PORT );
  http.connectSSL(SERVER, HTTPS_PORT); // Will halt if an error occurs
  Serial.println("OK");

  // Make a HTTP request
  http.addHeader("Accept", "text/html");
  http.addHeader("Connection", "keep-alive");

  Serial.printf("Requesting '%s' ... ", url);
  http.get(SERVER, url); // Will halt if an error occurs
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
