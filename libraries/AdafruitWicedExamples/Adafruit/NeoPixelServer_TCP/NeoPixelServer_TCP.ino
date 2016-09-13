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

/* This example will start a TCP server on the feather, polling
 * for any incoming messages and setting NeoPixels acdordingly.
 * To run this demo:
 * - Change SSID/Pass
 * - Compile and run
 * - Use a  TCP client on your PC such as netcat as follows
 *   'echo -n -e "\x08\x00\x00\xFF\x00" | nc 10.0.1.10 8888'
 *   Change the IP (10.0.1.10) and port (8888) as necessary
 * - Alternatively, use a GUI client like "Packet Sender" and send
 *   a TCP packet in HEX to the appropriat IP and PORT:
 *   https://packetsender.com/
 */

#include <adafruit_feather.h>
#include <Adafruit_NeoPixel.h>

#define WLAN_SSID            "xxxx"
#define WLAN_PASS            "xxxx"

#define PORT                 8888                     // The TCP port to use

AdafruitTCPServer tcpserver(PORT);

#define PIN                  PC7  // 4*8 Neopixel Wing uses PC7 by default
#define NUMPIXELS            32   // 32 pixels on https://www.adafruit.com/product/2945

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void disconnect_callback(void);

/**************************************************************************/
/*!
    @brief  Prints a hexadecimal value in plain characters
    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void PrintHex(const byte * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++)
  {
    Serial.print(F("0x"));
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      Serial.print(F("0"));
    Serial.print(data[szPos]&0xff, HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      Serial.print(F(" "));
    }
  }
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

  Serial.println("Neopixel TCP Server Example\r\n");

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

  // Starting server at defined port
  tcpserver.begin();

  // Initialize the NeoPixel library
  pixels.begin();

  Serial.print("Listening on port "); Serial.println(PORT);

  // Display a simple help message
  Serial.println("");
  Serial.println("*********************************************************************");
  Serial.println("");
  Serial.println("To update a neopixel send the following payload over TCP:");
  Serial.println("");
  Serial.println("  [U16:Pixel Number] [U8:Red] [U8:Green] [U8:Blue]");
  Serial.println("");
  Serial.println("To update pixel 8 to 100% green, for example, you would send:");
  Serial.println("");
  Serial.println("  08 00 00 FF 00");
  Serial.println("");
  Serial.println("Where:");
  Serial.println("");
  Serial.println("  0x0008 = Pixel number 8 (in little-endian notation 0x0008 = 08 + 00)");
  Serial.println("    0x00 = Red (0)");
  Serial.println("    0xFF = Green (255)");
  Serial.println("    0x00 = Blue (0)");
  Serial.println("");
  Serial.println("To update all pixels at once, send 0xFFFF (65535) as the pixel number");
  Serial.println("");
  Serial.println("*********************************************************************");
  Serial.println("");
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
    // read data
    len = client.read(buffer, 256);

    // Print data along with peer's info
    Serial.print("[RX] from ");
    Serial.print(client.remoteIP());
    Serial.printf(" port %d : ", client.remotePort());
    PrintHex(buffer, len);

    // Set the NeoPixel
    uint16_t pixel_num = 0;
    memcpy(&pixel_num, &buffer[0], 2);
    if (pixel_num == 0xFFFF)
    {
      //Serial.print("Setting all pixels");
      uint16_t i = 0;
      for (i = 0; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(buffer[2], buffer[3], buffer[4]));
      }
    }
    else
    {
      //Serial.print("Setting pixel "); Serial.println(pixel_num);
      pixels.setPixelColor(pixel_num, pixels.Color(buffer[2], buffer[3], buffer[4]));
    }
    pixels.show();

    // call stop() to free memory by Client
    client.stop();
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

