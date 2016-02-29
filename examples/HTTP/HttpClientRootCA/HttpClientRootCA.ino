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

/* How to run this example
* 1. Change ssid/pass to match your network
* 2. Choose the SERVER_ID or use your own server e.g www.adafruit.com
* If necessary:
*    3. cd to /tools/pycert and run pycert.py as follows:
*       $ python pycert.py download www.adafruit.com
*    4. The script will generate certificates.h, whicf holds the certificate
*       chain of the server. You may need to close and re-open this sketch to
*       reload it.
* 5. Compile and run this sketch
*/

#include <adafruit_feather.h>
#include <adafruit_http.h>
#include "certificates.h"

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPass"

#define HTTPS_PORT            443

#define S3_SERVER             "adafruit-download.s3.amazonaws.com"

// Some server such as facebook check the user_agent header to
// return data accordingly. Setting curl to mimics command line browser !!
// For list of popular user agent http://www.useragentstring.com/pages/useragentstring.php
#define USER_AGENT_HEADER    "curl/7.45.0"

int ledPin = PA15;

// Change the SERVER_ID to match the generated certificates.h
#define SERVER_ID    6

const char * server_arr[][2] =
{
    [0 ] = { "www.adafruit.com"     , "/" },
    [1 ] = { "twitter.com"          , "/" },
    [2 ] = { "www.google.com"       , "/" },
    [3 ] = { "www.facebook.com"     , "/" },
    [4 ] = { "www.yahoo.com"        , "/" },
    [5 ] = { "aws.amazon.com"        , "/" },

    // S3 server to test large files,
    [6] = { S3_SERVER, "/text_10KB.txt" },
    [7] = { S3_SERVER, "/text_100KB.txt"},
    [8] = { S3_SERVER, "/text_1MB.txt"  },
};

// You can set your own server & uri here
const char * server = server_arr[SERVER_ID][0];
const char * uri    = server_arr[SERVER_ID][1];

// Use the HTTP class
AdafruitHTTP http;

bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);

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

void receive_callback(void)
{
  // if there are incoming bytes available
  // from the server, read then print them:
  int c;
  while ( (c = http.read())> 0 )
  {
    Serial.write( (isprint(c) || iscntrl(c)) ? ((char)c) : '.');
  }
}

void disconnect_callback(void)
{
  Serial.println();
  Serial.println("---------------------");
  Serial.println("DISCONNECTED CALLBACK");
  Serial.println("---------------------");
  Serial.println();
}

void setup()
{
  Serial.begin(115200);

  // Wait for the Serial port to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("HTTP Client Callback Example");
  Serial.println();

  while( !connectAP() )
  {
    delay(500);
  }

  printWifiStatus();

  // Add Root CA Chain to pre-flashed chain (if necessary)
  //Feather.addRootCA(rootca_certs, ROOTCA_CERTS_LEN);

  // Tell the HTTP client to auto print error codes and halt on errors
  http.err_actions(true, true);

  // Set up callbacks
  http.setReceivedCallback(receive_callback);
  http.setDisconnectCallback(disconnect_callback);

  // Start a secure connection
  Serial.printf("Connecting to %s port %d ... ", server, HTTPS_PORT );
  http.connectSSL(server, HTTPS_PORT); // Will halt if an error occurs
  Serial.println("OK");

  // Make a HTTP request
  http.addHeader("User-Agent", USER_AGENT_HEADER);
  http.addHeader("Accept", "text/html");
  http.addHeader("Connection", "keep-alive");

  Serial.printf("Requesting '%s' ... ", uri);
  http.get(server, uri); // Will halt if an error occurs
  Serial.println("OK");
}

void loop()
{
  togglePin(ledPin);
  delay(250);
}

void printWifiStatus()
{
  // print the IP address and gateway:
  Serial.print("IP Address: ");
  Serial.println( IPAddress(Feather.localIP()) );
  Serial.print("Gateway : ");
  Serial.println( IPAddress(Feather.gatewayIP()) );

  // print the received signal strength:
  long rssi = Feather.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.println();
}
