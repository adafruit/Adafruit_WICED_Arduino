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

char ssid[] = "yourSSID";     //  your network SSID (name)
char pass[] = "yourPass";  // your network password (use for WPA, or use as key for WEP)

char uri[]    = "/";

// Change the SERVER_ID to match the generated certificates.h
#define SERVER_ID    1

#if SERVER_ID == 1
  char server[] = "www.adafruit.com";
#elif SERVER_ID == 2
  char server[] = "www.facebook.com";
#elif SERVER_ID == 3
  char server[] = "github.com";
#elif SERVER_ID == 4
  // hanged when print binary data
  char server[] = "twitter.com";
#elif SERVER_ID == 5
  // cannot connect
  char server[] = "www.google.com";
#elif SERVER_ID == 6
  char server[] = "www.yahoo.com";
#else
  #error Server is not defined
#endif

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
HTTPClient client;

void setup()
{
  Serial.begin(115200);

  // wait for Serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);
  
  do {
    Serial.print("Attempting to connect: ");
    Serial.println(ssid);
    // Connect using saved profile if possible since it re-connect much quicker
    if ( feather.checkProfile(ssid) )
    {
      feather.connect();
    }else
    {
      feather.clearProfiles();
      if ( feather.connect(ssid, pass) )
      {
        feather.saveConnectedProfile();
      }
    }
  } while( !feather.connected() );

  Serial.println("Connected to wifi");
  printWifiStatus();

  // Setting Certificates chain of the server
  Serial.print("Setting Root CA chain ... ");
  if ( feather.setRootCertificatesDER(root_certs, sizeof(root_certs)) )
  {
    Serial.println("done");
  }else
  {
    Serial.println("failed");
  }

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
    Serial.println("Failed: %s (%d)", client.errstr(), client.errno());
    Serial.println();
  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while ( client.available() )
  {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if ( !client.connected() )
  {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true) delay(1);
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





