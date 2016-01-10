/*********************************************************************
 This is an example for our Feather WiFi modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <WiFiFeather.h>

char ssid[] = "yournetwork";     //  your network SSID (name)
char pass[] = "yourpassword";  // your network password (use for WPA, or use as key for WEP)

// your local PC's IP to test the throughput
// Run this command to create a server on your PC
// nc -l 8888 
IPAddress server_ip(192, 168, 0, 20);
const uint16_t port = 8888;

WiFiClient client;

const char * testData[10] =
{
    "000000000000000000000000000000000000000000000000\r\n",
    "111111111111111111111111111111111111111111111111\r\n",
    "222222222222222222222222222222222222222222222222\r\n",
    "333333333333333333333333333333333333333333333333\r\n",
    "444444444444444444444444444444444444444444444444\r\n",
    "555555555555555555555555555555555555555555555555\r\n",
    "666666666666666666666666666666666666666666666666\r\n",
    "777777777777777777777777777777777777777777777777\r\n",
    "888888888888888888888888888888888888888888888888\r\n",
    "999999999999999999999999999999999999999999999999\r\n",
};

const uint32_t testStepLen = strlen(testData[0]);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    delay(1); // wait for serial port to connect. Needed for native USB port only
  }
  
  // attempt to connect to Wifi network:
  do{
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
  }while( WL_CONNECTED != WiFi.begin(ssid, pass) );

  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.print("\nStarting connection to ");
  Serial.print(server_ip);
  Serial.printf(":%d\r\n", port);
  
  client.usePacketBuffering(true); // use packet buffering for maximum performance
  client.connect(server_ip, port);
}

void loop()
{
  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println("not connected or disconncted.");
    client.stop();

    // do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  
  uint32_t start, stop, sent_loop;
  uint32_t remaining_loop = 10000; // number of loop to send, each loop is testStepLen
  start = stop = sent_loop = 0;

  Serial.println("Enter any key to start Sending test");
  char inputs[64];
  getUserInput(inputs, sizeof(inputs));

  start = millis();
  while (remaining_loop > 0)
  {
    client.write( testData[sent_loop%10] );

    sent_loop++;
    remaining_loop--;
  }

  // Data is buffered and only be sent when network packet is full
  // or flush() is called to optimize network usage
  client.flush();

  // print result
  stop = millis() - start;
  Serial.printf("Sent %d bytes in %.02f seconds.\r\n", sent_loop*testStepLen, stop/1000.0F);


  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
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

void getUserInput(char buffer[], uint8_t maxSize)
{
  memset(buffer, 0, maxSize);
  while( Serial.peek() < 0 ) { delay(1); }

  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
  } while( (count < maxSize) && !(Serial.peek() < 0) );
}
