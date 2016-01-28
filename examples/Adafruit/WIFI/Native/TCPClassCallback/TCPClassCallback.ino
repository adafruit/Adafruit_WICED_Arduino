#include "adafruit_wifi.h"
#include "adafruit_tcp.h"

char const     ssid[ ]   = "SSIDNAME";             // Your network SSID (name)
char const     pass[ ]   = "PASSWORD";             // Your network password (use for WPA, or use as key for WEP)
char const     server[ ] = "www.adafruit.com";     // The TCP server to connect to
const uint16_t port      = 80;                     // The TCP port to use
char const     page[ ]   = "/testwifi/index.html"; // The HTTP resource to request

AdafruitTCP adatcp;

int receive_callback(void* arg1, void* arg2)
{
  (void) arg1; // reserved for future use
  (void) arg2; // reserved for future use

  Serial.println("Receive callback");

  // if there are incoming bytes available
  // from the server, read then print them:
  while (adatcp.available())
  {
    char c = adatcp.read();
    Serial.write(c);
  }

  return 0;
}

int disconnect_callback(void* arg1, void* arg2)
{
  (void) arg1; // reserved for future use
  (void) arg2; // reserved for future use

  Serial.println();
  Serial.println("disconnect_callback.");

  adatcp.stop();

  return 0;
}

void setup()
{
  //Initialize Serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
    // wait for Serial port to connect. Needed for native USB port only
    delay(1);
  }

  Serial.println("TCP Example With Callback\r\n");

  // Attempt to connect to the Wifi network:
  do
  {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
  } while( !feather.connect(ssid, pass) ) ;

  Serial.println("Connected!");
  Serial.println("\nStarting connection to server...");

  // Install callback
  adatcp.setReceivedCallback(receive_callback);
  adatcp.setDisconnectCallback(disconnect_callback);

  // If we can connect to the TCP Server, report it via Serial.print
  if (adatcp.connect(server, port))
  {
    Serial.println("Connected to server");
    // Make an HTTP request:
    adatcp.printf("GET %s HTTP/1.1\r\n", page);
    adatcp.print("Host: "); adatcp.println(server);
    adatcp.println("Connection: close");
    adatcp.println();

    // Data is buffered and will only be sent when the network packet is full
    // or flush() is called to optimize network usage
    // adatcp.flush();
  } else
  {
    Serial.println("Failed to connect to server");
  }
}

void loop()
{
}
