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

/* This example will start a UDP server on the feather, registers
 * a receive callback, and echoes back any incoming messages.
 * To run this demo:
 * - Change SSID/Pass
 * - Compile and run
 * - Use a UDP client on your PC such as netcast as follows:
 *   'nc -u IP port', e.g your Feather's IP is 192.168.1.100
 *   and LOCAL_PORT is 8888 then
 *     > nc -u 192.168.1.100 8888
 *     > <Type your message>
 */

#include <adafruit_feather.h>
#include <adafruit_featherap.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"
#define WLAN_ENCRYPTION       ENC_TYPE_WPA2_AES
#define WLAN_CHANNEL          1

#define LOCAL_PORT           8888

IPAddress apIP     (192, 168, 2, 1);
IPAddress apGateway(192, 168, 2, 1);
IPAddress apNetmask(255, 255, 255, 0);

AdafruitUDP udp(WIFI_INTERFACE_SOFTAP);

char packetBuffer[255];

/**************************************************************************/
/*!
    @brief  Received something from the UDP port
*/
/**************************************************************************/
void received_callback(void)
{
  int packetSize = udp.parsePacket();

  if ( packetSize )
  {
    // Print out the contents with remote information
    Serial.printf("Received %d bytes from ", packetSize);
    Serial.print( IPAddress(udp.remoteIP()) );
    Serial.print( " : ");
    Serial.println( udp.remotePort() );

    udp.read(packetBuffer, sizeof(packetBuffer));
    Serial.print("Contents: ");
    Serial.write(packetBuffer, packetSize);
    Serial.println();

    // Echo back contents
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(packetBuffer, packetSize);
    udp.endPacket();
  }
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the Serial Monitor to open
  while (!Serial)
  {
    /* Delay required to avoid RTOS task switching problems */
    delay(1);
  }

  Serial.println("SoftAP UDP Echo Server Callback Example\r\n");

  // Print all software versions
  Feather.printVersions();

  Serial.println("Configuring SoftAP\r\n");
  FeatherAP.err_actions(true, true);
  FeatherAP.begin(apIP, apGateway, apNetmask, WLAN_CHANNEL);

  Serial.println("Starting SoftAP\r\n");
  FeatherAP.start(WLAN_SSID, WLAN_PASS, WLAN_ENCRYPTION);
  FeatherAP.printNetwork();

  // Tell the UDP client to auto print error codes and halt on errors
  udp.err_actions(true, true);

  // Set the RX callback handler
  udp.setReceivedCallback(received_callback);

  // Start the UDP server
  Serial.printf("Openning UDP at port %d ... ", LOCAL_PORT);
  udp.begin(LOCAL_PORT);
  Serial.println("OK");

  Serial.println("Please use your PC/mobile and send any text to ");
  Serial.print( apIP );
  Serial.print(" UDP port ");
  Serial.println(LOCAL_PORT);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
}
