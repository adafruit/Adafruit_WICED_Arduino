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

/* This example will start a TCP server on the feather, registering
   a 'receive' callback and echoing back any incoming messages. To
   run this demo:
   - Change SSID/Pass
   - Compile and run
   - Use a TCP client on your PC such as netcast as follows:
    'echo "your message" | nc IP port'. e.g your Feather's IP is 192.168.1.100
     and PORT is 8888 then
       > echo "Hello Feather" | nc 192.168.100 8888
*/

#include <adafruit_feather.h>

#define WLAN_SSID            "xxxx"
#define WLAN_PASS            "xxxx"

#define PORT                 23                     // The TCP port to use.  23 is telnet

AdafruitTCPServer tcpserver(PORT);

#define MAX_CLIENTS 3
AdafruitTCP clientList[MAX_CLIENTS];  //can have MAX_CLIENTS simultaneous clients
int freeClientIndex = 0;  //init to first client slot

int ledPin = PA15;

void disconnect_callback(void);

/**************************************************************************/
/*!
    @brief  This callback is fired when there is a connection request from
            a TCP client. Use accept() to establish the connection and
            retrieve the client 'AdafruitTCP' instance.
*/
/**************************************************************************/
void connect_request_callback(void)
{
  uint8_t buffer[256];
  uint16_t len;

  if (freeClientIndex < MAX_CLIENTS)  //any free client slots left?
  {
    clientList[freeClientIndex] = tcpserver.available();  //get the new client

    if ( clientList[freeClientIndex]  )  //was that successful?
    {
      freeClientIndex++;   //increment to next client slot
    }
  }
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the serial port to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  pinMode(ledPin, OUTPUT); // Init LED pin

  Serial.println("TCP Server Example (Callbacks)\r\n");

  // Print all software versions
  Feather.printVersions();

  // Set disconnection callback
  Feather.setDisconnectCallback(disconnect_callback);

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the TCP Server to auto print error codes and halt on errors
  tcpserver.err_actions(true, true);

  // Setup callbacks: must be done before begin()
  tcpserver.setConnectCallback(connect_request_callback);

  // Starting server at defined port
  tcpserver.begin();

  Serial.print("Listening on port "); Serial.println(PORT);
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  uint8_t buffer[256];
  uint16_t len;

  for (int index = 0; index < MAX_CLIENTS; index++)  //scan clients for input
  {
    if (NULL != clientList[index])  //is this a valid client?
    {
      if (clientList[index].available())  //any data available?
      {
        digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        len = clientList[index].read(buffer, 256);   //read client input
        Serial.print("[RX:"); // Display the incoming message and source in Serial Monitor
        Serial.print(clientList[index].remoteIP());
        Serial.print("] ");
        Serial.write(buffer, len);
        digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
        for (int receiverindex = 0; receiverindex < MAX_CLIENTS; receiverindex++)  //look for receiving clients
        {
          if ((receiverindex!= index) && (NULL != clientList[receiverindex]))  //if it's a valid client, and it's not the sending client
          {
            clientList[receiverindex].write(buffer, len);  //send the data
          }
        }
      }
    }
  }
}

/**************************************************************************/
/*!
    @brief  Connect to the pre-defined access point
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

/**************************************************************************/
/*!
    @brief  AP disconnect callback
*/
/**************************************************************************/
void disconnect_callback(void)
{
  Serial.println();
  Serial.println("------------------------");
  Serial.println("AP DISCONNECTED CALLBACK");
  Serial.println("------------------------");
  Serial.println();
}

