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
#include <adafruit_featherap.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"
#define WLAN_ENCRYPTION       ENC_TYPE_WPA2_AES
#define WLAN_CHANNEL          1

#define PORT                  23                     // The TCP port to use.  23 is telnet
#define MAX_CLIENTS           3

IPAddress apIP     (192, 168, 2, 1);
IPAddress apGateway(192, 168, 2, 1);
IPAddress apNetmask(255, 255, 255, 0);

AdafruitTCPServer tcpserver(PORT, WIFI_INTERFACE_SOFTAP);
AdafruitTCP clientList[MAX_CLIENTS];  //can have MAX_CLIENTS simultaneous clients

int ledPin = PA15;

/**************************************************************************/
/*!
    @brief  This callback is fired when there is a connection request from
            a TCP client. Use accept() to establish the connection and
            retrieve the client 'AdafruitTCP' instance.
*/
/**************************************************************************/
void connect_request_callback(void)
{
  for(int i=0; i<MAX_CLIENTS; i++)
  {
    // find a free slot in client list
    if ( !clientList[i] )
    {
      //get the new client
      clientList[i] = tcpserver.available();

      if ( clientList[i] ) //was that successful?
      {
        Serial.print("Client ");
        Serial.print(i);
        Serial.println(" connected");
                
        // Set disconnect callback to free up resource
        // NOTE: Resource is also automatically freed up when client
        // disconnected even if we don't explicitly call stop()
        clientList[i].setDisconnectCallback(client_disconnect_callback);
      }  

      break;
    }
  }
}

/**************************************************************************/
/*!
    @brief  This callback is fired when client disconnect from server
*/
/**************************************************************************/
void client_disconnect_callback(void)
{
  // All clients in the list share the same disconnect callback
  // Scan the list to find existed client but not connected to free it up
  for(int i=0; i<MAX_CLIENTS; i++)
  {
    if ( clientList[i] && !clientList[i].connected() )
    {
      clientList[i].stop();

      Serial.print("Client ");
      Serial.print(i);
      Serial.println(" disconnected");
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

  Serial.println("SoftAP TCP Chat Server Example\r\n");

  // Print all software versions
  Feather.printVersions();

  Serial.println("Configuring SoftAP\r\n");
  FeatherAP.err_actions(true, true);
  FeatherAP.begin(apIP, apGateway, apNetmask, WLAN_CHANNEL);

  Serial.println("Starting SoftAP\r\n");
  FeatherAP.start(WLAN_SSID, WLAN_PASS, WLAN_ENCRYPTION);

  // Tell the TCP Server to auto print error codes and halt on errors
  tcpserver.err_actions(true, true);
  
  // Setup callbacks: must be done before begin()
  tcpserver.setConnectCallback(connect_request_callback);

  // Starting server at defined port
  Serial.print("Listening on port "); Serial.println(PORT);
  tcpserver.begin();
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
    if ( clientList[index] )  //is this a valid client?
    {
      if (clientList[index].available())  //any data available?
      {
        digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        len = clientList[index].read(buffer, 256);   //read client input
        
        // Display the incoming message and source in Serial Monitor
        Serial.print("[RX:"); 
        Serial.print(clientList[index].remoteIP());
        Serial.print("] Client "); Serial.print(index); Serial.print(": ");
        
        Serial.write(buffer, len);
        Serial.println();
        digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
        for (int receiverindex = 0; receiverindex < MAX_CLIENTS; receiverindex++)  //look for receiving clients
        {
          if ((receiverindex!= index) && clientList[receiverindex])  //if it's a valid client, and it's not the sending client
          {
            clientList[receiverindex].write(buffer, len);  //send the data
          }
        }
      }
    }
  }
}
