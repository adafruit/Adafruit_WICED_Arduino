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

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPass"

#define LOCAL_PORT           8888

AdafruitUDP udp;

char packetBuffer[255];

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

void setup()
{
  Serial.begin(115200);

  // wait for Serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("UDP Echo Callback Example");
  Serial.println();

  while( !connectAP() )
  {
    delay(500);
  }

  // Tell the UDP client to auto print error codes and halt on errors
  udp.err_actions(true, true);
  udp.setReceivedCallback(received_callback);

  Serial.printf("Openning UDP at port %d ... ", LOCAL_PORT);
  udp.begin(LOCAL_PORT);
  Serial.println("OK");

  Serial.println("Please use your PC/mobile and send any text to ");
  Serial.print( IPAddress(Feather.localIP()) );
  Serial.print(" UDP port ");
  Serial.println(LOCAL_PORT);
}

void loop()
{

}

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
