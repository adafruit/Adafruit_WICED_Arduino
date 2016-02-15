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

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPass"
#define LOCAL_SERVER         "192.168.0.20"

int ledPin = PA15;

// Change the SERVER_ID to match the generated certificates.h
#define SERVER_ID    12

const char * server_arr[][2] =
{
    [0 ] = { "www.adafruit.com"     , "/" },
    [1 ] = { "www.google.com"       , "/" },
    [2 ] = { "www.arduino.cc"       , "/" },
    [3 ] = { "www.yahoo.com"        , "/" },
    [4 ] = { "www.microsoft.com"    , "/" },
    [5 ] = { "www.reddit.com"       , "/" },
    [6 ] = { "news.ycombinator.com" , "/" },
    [7 ] = { "www.facebook.com"     , "/" },
    [8 ] = { "www.geotrust.com"     , "/" },
    [9 ] = { "www.eff.org"          , "/" },
    [10] = { "www.twitter.com"      , "/" },
    [11] = { "www.flickr.com"       , "/" },

    // Local server, 
    [12] = { LOCAL_SERVER, "text_1KB.txt"  },
    [13] = { LOCAL_SERVER, "text_10KB.txt" },
    [14] = { LOCAL_SERVER, "text_100KB.txt"},
    [15] = { LOCAL_SERVER, "text_1MB.txt"  },
    
};

// You can set your own server & uri here
const char * server = server_arr[SERVER_ID][0];
const char * uri    = server_arr[SERVER_ID][1];

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
HTTPClient client;

bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);

  // Connect using saved profile if possible since it re-connect much quicker
  if ( feather.checkProfile(WLAN_SSID) )
  {
    feather.connect();
  }else
  {
    feather.clearProfiles();
    if ( feather.connect(WLAN_SSID, WLAN_PASS) )
    {
      feather.saveConnectedProfile();
    }
  }

  if ( feather.connected() )
  {
    Serial.println("Connected!");
  } else
  {
    Serial.printf("Failed! %s (%d)", feather.errstr(), feather.errno());
  }
  
  Serial.println();
  return feather.connected();
}

int receive_callback(void* arg1, void* arg2)
{
  (void) arg1; // reserved for future use
  (void) arg2; // reserved for future use
  
  // if there are incoming bytes available
  // from the server, read then print them:
  while (client.available())
  {
    char c = client.read();
    Serial.write( (isprint(c) || iscntrl(c)) ? c : '.');
  }

  return 0;
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
  client.setReceivedCallback(receive_callback);

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
  togglePin(ledPin);
  delay(250);
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

