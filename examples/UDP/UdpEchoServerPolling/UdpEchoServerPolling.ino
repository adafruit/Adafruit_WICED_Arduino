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

/* This example will start a UDP server on the feather, polling
 * for any incoming messages and echoing incoming data back.
 * To run this demo:
 * To run this demo:
 * - Change SSID/Pass
 * - Compile and run
 * - Use a UDP client on your PC such as netcast as follows:
 *   'nc -u IP port', e.g your Feather's IP is 192.168.1.100
 *   and LOCAL_PORT is 8888 then
 *     > nc -u 192.168.100 8888
 *     > <Type your message>
 */

#include <adafruit_feather.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"
#define LOCAL_PORT           8888

AdafruitUDP udp;
char packetBuffer[255];

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // wait for the serial port to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("UDP Echo Server Polling Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();


  // Tell the UDP client to auto print error codes and halt on errors
  udp.err_actions(true, true);

  // Start the UDP server
  Serial.printf("Openning UDP at port %d ... ", LOCAL_PORT);
  udp.begin(LOCAL_PORT);
  Serial.println("OK");

  Serial.println("Please use your PC/mobile and send any text to ");
  Serial.print( IPAddress(Feather.localIP()) );
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
    @brief  Connect to defined Access Point
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
