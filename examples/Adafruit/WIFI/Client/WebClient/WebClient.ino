/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

#include <SPI.h>
//#include <Ethernet.h>
#include <EthernetClient.h>
#include "adafruit_wifi.h"

#define WLAN_SSID            "SSID of AP"
#define WLAN_PASS            "Password of AP"

#define URI                 "/testwifi/index.html"
#define HOST                "www.adafruit.com"

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    delay(1); // wait for serial port to connect.
  }
  
 connectAP();

  // if you get a connection, report back via serial:
  if (client.connect(HOST, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET " URI);
    client.println(" HTTP/1.1");
    client.println("Host: " HOST);
    client.println("Connection: close");
    client.println();
    client.flush();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  char c = client.read();
  if (c != EOF) {
    Serial.print(c);
  }else
  {
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
  
      // do nothing forevermore:
      while (true);
    }
  }
}


/**************************************************************************/
/*!
    @brief  Connect to pre-specified AP

    @return Error code
*/
/**************************************************************************/
int connectAP()
{
  // Attempt to connect to an AP
  Serial.print(F("Attempting to connect to: "));
  Serial.println(WLAN_SSID);

  int error = feather.connectAP(WLAN_SSID, WLAN_PASS);

  if (error == 0)
  {
    Serial.println(F("Connected!"));
  }
  else
  {
    Serial.print(F("Failed! Error: "));
    Serial.println(error, HEX);
    while(1) delay(1);
  }
  Serial.println();
}

