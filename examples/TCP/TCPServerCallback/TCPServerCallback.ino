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

#include <adafruit_feather.h>

#define WLAN_SSID            "thach"
#define WLAN_PASS            "thach367"

#define PORT                 80                     // The TCP port to use

AdafruitTCPServer tcpsever(1);

/**************************************************************************/
/*!
    @brief  TCP Server connect callback
*/
/**************************************************************************/
void connect_request_callback(void)
{
  DBG_LOCATION();
}

/**************************************************************************/
/*!
    @brief  TCP Server received callback
*/
/**************************************************************************/
void receive_callback(void)
{
  DBG_LOCATION();
}

/**************************************************************************/
/*!
    @brief  TCP Server disconnect callback
*/
/**************************************************************************/
void disconnect_callback(void)
{
  Serial.println();
  Serial.println("---------------------");
  Serial.println("DISCONNECTED CALLBACK");
  Serial.println("---------------------");
  Serial.println();
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the serial port to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("TCP Client Example (Callbacks)\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the TCP Server to auto print error codes and halt on errors
  tcpsever.err_actions(true, true);

  // Setup callbacks: must be done before begin()
  tcpsever.setConnectCallback(connect_request_callback);
  tcpsever.setReceivedCallback(receive_callback);
  tcpsever.setDisconnectCallback(disconnect_callback);

  // Starting server at defined port
  tcpsever.begin(PORT);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
}

/**************************************************************************/
/*!
    @brief  Connect to defined Access Point
*/
/**************************************************************************/
bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);

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
