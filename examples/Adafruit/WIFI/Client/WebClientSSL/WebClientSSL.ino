/**************************************************************************/
/*!
    @file     WebClientSSL.ino
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2015, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

/* How to run this example
* 1. Change ssid/pass to match your network
* 2. Choose the SERVER_ID or use your own server e.g www.adafruit.com
* 3. cd to this folder & and get_certificates.py script as follows
*      $ python get_certificates.py www.adafruit.com
* 4. The script will genearate certificates.h contains certificate chain
* of the server. You may need to close and re-open this sketch to reload
* 5. Compile and run this sketch
*/

#include <WiFiFeather.h>
#include <WiFiFeatherClient.h>
#include <AdafruitNet.h>
#include "certificates.h"

char ssid[] = "yourNetwork";     //  your network SSID (name)
char pass[] = "secretPassword";  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

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

void setup() {
  //Initialize Serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) 
  {
    // wait for Serial port to connect. Needed for native USB port only
    delay(1);
  }
  
  // Connect using saved profile since it re-connect much quicker
  if ( !WiFi.checkProfile(ssid) )
  {
    WiFi.clearProfiles();
    WiFi.addProfile(ssid, pass, ENC_TYPE_WPA2_AES);
  }
  
  do {
    Serial.println("Attempting to connect");
  } while( WiFi.begin() != WL_CONNECTED);

  Serial.println("Connected to wifi");
  printWifiStatus();

  // Setting Certificates chain of the server
  Serial.print("Setting Root CA chain ... ");
  if ( WiFi.setRootCertificatesDER(root_certs, sizeof(root_certs)) )
  {
    Serial.println("done");
  }else
  {
    Serial.println("failed");
  }

  Serial.print("\nStarting connection to server...");
  Serial.println(server);

  client.setTimeout(10000);
  int err = client.connectSSL(server, 443);
  if ( err > 0) {
    Serial.println("connected to server");
    
    // Make a HTTP request:
    client.get(server, uri);
  }else
  {
    Serial.println("failed to connect");
    Serial.printf("error = %d", err); 
  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
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
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}





