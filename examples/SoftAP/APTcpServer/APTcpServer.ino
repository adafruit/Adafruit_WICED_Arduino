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
 * a 'receive' callback and echoing back any incoming messages. To
 * run this demo:
 * - Change SSID/Pass
 * - Compile and run
 * - Use a TCP client on your PC such as netcast as follows:
 *  'echo "your message" | nc IP port'. e.g your Feather's IP is 192.168.1.100
 *   and PORT is 8888 then
 *     > echo "Hello Feather" | nc 192.168.100 8888
 */

#include <adafruit_feather.h>
#include <adafruit_featherap.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"
#define WLAN_ENCRYPTION       ENC_TYPE_WPA2_AES
#define WLAN_CHANNEL          1

// The TCP port to use
#define PORT                 8888

IPAddress apIP     (192, 168, 2, 1);
IPAddress apGateway(192, 168, 2, 1);
IPAddress apNetmask(255, 255, 255, 0);

AdafruitTCPServer tcpserver(PORT, WIFI_INTERFACE_SOFTAP);

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("SoftAP TCP Server Example\r\n");

  Serial.println("Configuring SoftAP\r\n");
  FeatherAP.err_actions(true, true);
  FeatherAP.begin(apIP, apGateway, apNetmask, WLAN_CHANNEL);

  Serial.println("Starting SoftAP\r\n");
  FeatherAP.start(WLAN_SSID, WLAN_PASS, WLAN_ENCRYPTION);

  // Starting server at defined port
  Serial.print("Listening on port "); Serial.println(PORT);
  tcpserver.begin();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  uint8_t buffer[256];
  uint16_t len;

  AdafruitTCP client = tcpserver.available();

  if ( client )
  {
    delay(100);
   
    // read data
    len = client.read(buffer, 256);
    
    // Print data along with peer's info
    Serial.print("[RX] from ");
    Serial.print(client.remoteIP());
    Serial.printf(" port %d : ", client.remotePort());
    Serial.write(buffer, len);
    Serial.println();

    // Echo back
    client.write(buffer, len);

    // call stop() to free memory by Client
    client.stop();
  }
}
