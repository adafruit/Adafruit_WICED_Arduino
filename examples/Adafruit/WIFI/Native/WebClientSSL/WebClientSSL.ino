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
* 
* NOTE: to create self-signed certificate for localhost
* https://gist.github.com/sl4m/5091803
*/

#include <adafruit_feather.h>
#include <adafruit_tcp.h>
#include <AdafruitNet.h>
#include "certificates.h"

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPass"

#define HTTPS_PORT            443

#define LOCAL_SERVER         "192.168.0.21"


// Some server such as facebook check the user_agent header to
// return data accordingly. Setting curl to mimics command line browser !!
// For list of popular user agent http://www.useragentstring.com/pages/useragentstring.php
#define USER_AGENT_HEADER    "User-Agent: curl/7.45.0"

int ledPin = PA15;

// Change the SERVER_ID to match the generated certificates.h
#define SERVER_ID    2

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
    [10] = { "twitter.com"          , "/" },
    [11] = { "www.flickr.com"       , "/" },

    // Local server, 
    [12] = { LOCAL_SERVER, "/text_10KB.txt" },
    [13] = { LOCAL_SERVER, "/text_100KB.txt"},
    [14] = { LOCAL_SERVER, "/text_1MB.txt"  },
    
};

// You can set your own server & uri here
const char * server = server_arr[SERVER_ID][0];
const char * uri    = server_arr[SERVER_ID][1];

// Use the HTTP class
HTTPClient http;

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

void receive_callback(AdafruitTCP* pTCP)
{ 
  // if there are incoming bytes available
  // from the server, read then print them:
#if 1
  while (pTCP->available() > 0 )
  {
    char c = pTCP->read();
    Serial.write( (isprint(c) || iscntrl(c)) ? c : '.');
  }
#else
  int c;
  while ( (c = pTCP->read())> 0 )
  {
    Serial.write( (isprint(c) || iscntrl(c)) ? ((char)c) : '.');
  }
#endif
}

void disconnect_callback(AdafruitTCP* pTCP)
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

  // wait for Serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);
  
  Serial.println("WebClientSSL Example");
  Serial.println();

  while( !connectAP() )
  {
    delay(100);
  }

  printWifiStatus();

#if 0 // currently disabl Certificate verification
  // Setting Certificates chain of the server
  Serial.print("Setting Root CA chain ... ");
  if ( feather.setRootCertificatesDER(root_certs, sizeof(root_certs)) )
  {
    Serial.println("done");
  }else
  {
    Serial.println("failed");
  }
#endif
  
  // Disable certificate verification (accept any server)
  feather.tlsRequireVerification(false);

  Serial.printf("\r\nStarting connection to %s port %d...\r\n", server,HTTPS_PORT );
  
  http.setTimeout(10000);
  http.setReceivedCallback(receive_callback);
  http.setDisconnectCallback(disconnect_callback);

  if ( http.connectSSL(server, HTTPS_PORT) )
  {
    Serial.println("connected to server");
    
    // Make a HTTP request:
    http.addHeader(USER_AGENT_HEADER);
    http.addHeader("Accept: text/html"); // only accept text field
    http.addHeader("Connection: keep-alive");
    http.get(server, uri);
  }else
  {
    Serial.printf("Failed: %s (%d)", http.errstr(), http.errno());
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

