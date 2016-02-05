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
* 3. cd to this folder & and get_certificates.py script as follows
*      $ python get_certificates.py www.adafruit.com
* 4. The script will genearate certificates.h contains certificate chain
* of the server. You may need to close and re-open this sketch to reload
* 5. Compile and run this sketch
*/

#include <adafruit_wifi.h>
#include <adafruit_tcp.h>
#include <AdafruitNet.h>
#include "certificates.h"

#define WLAN_SSID            "thach"
#define WLAN_PASS            "thach367"

char uri[]    = "/";

// Change the SERVER_ID to match the generated certificates.h
#define SERVER_ID    2

const char * server_arr[] =
{
    [0] = "www.adafruit.com"  ,
    [1] = "www.google.com"    ,
    [2] = "www.arduino.cc"    ,
    [3] = "www.yahoo.com"     ,
    [4] = "www.microsoft.com" ,
    [5] = "www.reddit.com"    ,
    [6] = "www.facebook.com"  ,
    [7] = "www.geotrust.com"  ,
    [8] = "www.eff.org"       ,
    [9] = "www.twitter.com"   ,
    [10] = "www.flickr.com"   ,
};

const char * server = server_arr[SERVER_ID];

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
HTTPClient client;

bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);

  if ( feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.printf("Failed! %s (%d)", feather.errstr(), feather.errno());
    Serial.println();
  }
  Serial.println();

  return feather.connected();
}

void setup()
{
  Serial.begin(115200);

  // wait for Serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);
  
  Serial.println("WebClientSSL Example");

  while( !connectAP() )
  {
    delay(100);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

#if 0
  // Setting Certificates chain of the server
  Serial.print("Setting Root CA chain ... ");
  if ( feather.setRootCertificatesDER(root_certs, sizeof(root_certs)) )
  {
    Serial.println("done");
  }else
  {
    Serial.println("failed");
  }
#else
  feather.tlsRequireVerification(false);
#endif  

  Serial.print("\nStarting connection to server...");
  Serial.println(server);

  client.setTimeout(10000);

  if ( client.connectSSL(server, 443) )
  {
    Serial.println("connected to server");
    
    // Make a HTTP request:
    client.get(server, uri);
  }else
  {
    Serial.printf("Failed: %s (%d)", client.errstr(), client.errno());
    Serial.println();
  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  if ( client.available() )
  {
    char c = client.read();
    Serial.write( isprint(c) ? c : '.');
  }else
  {
    delay(1);
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(feather.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = feather.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = feather.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

