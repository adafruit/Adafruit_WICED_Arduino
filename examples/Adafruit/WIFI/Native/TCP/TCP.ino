#include "adafruit_wifi.h"
#include "adafruit_tcp.h"

char const ssid[] = "yournetwork";     //  your network SSID (name)
char const pass[] = "yourpassword";  // your network password (use for WPA, or use as key for WEP)
char const server[] = "www.adafruit.com";

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
AdafruitTCP client;

int receive_callback(void* arg1, void* arg2)
{
  (void) arg1; // reserve for future
  (void) arg2; // reserve for future
  
  Serial.println("receive callback");
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  
  return 0;
}

int disconnect_callback(void* arg1, void* arg2)
{
  (void) arg1; // reserve for future
  (void) arg2; // reserve for future
  
  Serial.println();
  Serial.println("disconnect_callback.");
  
  client.close();
  
  return 0;
}

void setup() {
  //Initialize Serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
    ; // wait for Serial port to connect. Needed for native USB port only
  }

  Serial.println("HTTP With Callback Example\r\n");

  // attempt to connect to Wifi network:
  do
  {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
  } while( feather.connectAP(ssid, pass) != ERROR_NONE ) ;

  Serial.println("Connected to wifi");
  Serial.println("\nStarting connection to server...");
  
  // Install callback
  client.setReceivedCallback(receive_callback);
  client.setDisconnectCallback(disconnect_callback);
  
  // if you get a connection, report back via Serial:
  if (client.connect(server, 80)) 
  {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /testwifi/index.html HTTP/1.1");
    client.println("Host: www.adafruit.com");
    client.println("Connection: close");
    client.println();
    
    // Data is buffered and only be sent when network packet is full
    // or flush() is called to optimize network usage
    // client.flush();
  }else
  {
    Serial.println("Failed to connect to server");
  }
}

void loop() {
  
  // Change to #if 1 if not using RX callback
#if 0  
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
    client.close();

    // do nothing forevermore:
    while (true) delay(1);
  }
#endif  
}






