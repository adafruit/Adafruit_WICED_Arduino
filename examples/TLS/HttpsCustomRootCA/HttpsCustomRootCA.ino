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

/* This example attempts tp access a secure site whose RootCA is not
 * covered by the default RootCA list. It demonstrates the ability to
 * add user RootCAs.
 *
 * The default RootCA list can be included (linked with custom
 * RootCAs if necessary) or you can disable the default list to save
 * memory.
 *
 * To run the demo:
 *
 * 1. Change the ssid/pass defines to match your access point
 * 2. Change the SERVER you want to connect e.g www.reddit.com
 * 3. Do one of following:
 *    - Use tools/pycert/pycert.py to download & generate the
        target server's certificates:
 *      $ python pycert.py download www.reddit.com
 *    - Download the root certificate using your browser in PEM
        format and convert it:
 *      $ python pycert.py convert DigiCertGlobalRootCA.pem
 *        Note: Root CA of www.reddit.com is DigiCertGlobalRootCA
 * 4. The script will generate certificates.h, which contains the
 *    certificate chain of the server. Copy the generated header to
 *    this sketch's folder.
 * 5. Open and run this sketch
*/

#include <adafruit_feather.h>
#include <adafruit_http.h>
#include "certificates.h"

#define WLAN_SSID               "yourSSID"
#define WLAN_PASS               "yourPass"

#define SERVER                  "www.reddit.com"
#define PAGE                    "/"
#define HTTPS_PORT              443

// RootCAs require a lot of SRAM to manage (~900 bytes for each certificate
// in the chain). The default RootCA has 5 certificates, so ~4.5 KB of
// FeatherLib's SRAM is used to manage them.
// A lack of memory could cause FeatherLib to malfunction in some cases.
// It is advised to disable the default RootCA list if you only need to
// connect to one specific website (or sites where the RootCA is not
// included in the default root certificate chain).
#define INCLUDE_DEFAULT_ROOTCA  0

// Some servers such as Facebook check the user_agent header to
// return data accordingly. Setting curl to mimics command line browser.
// For a list of popular user agent see:
// http://www.useragentstring.com/pages/useragentstring.php
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
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when coming out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the serial port to connect. Only needed for native USB CDC.
  while (!Serial) delay(1);
  
  Serial.println("HTTPS Custom Root CA Example");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // Small delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Include default RootCA if necessary
  Feather.useDefaultRootCA(INCLUDE_DEFAULT_ROOTCA);

  // Add custom RootCA since target server is not covered by default list
  Feather.addRootCA(rootca_certs, ROOTCA_CERTS_LEN);
 
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
  http.addHeader("User-Agent", USER_AGENT_HEADER);
  http.addHeader("Accept", "text/html");
  http.addHeader("Connection", "keep-alive");

  Serial.printf("Requesting '%s' ... ", PAGE);
  http.get(SERVER, PAGE); // Will halt if an error occurs
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
