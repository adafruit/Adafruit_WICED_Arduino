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

/* This example will read any input from Serial Monitor and
 * send it to the defined UDP server. To run this demo:
 * - Change SSID/Pass
 * - Change server_ip to your PC's IPv4 address
 * - Run 'nc -lu 8888' to start a UDP server on your PC
 * - Compile and run the sketch
 * - Enter a message in the Serial Monitor and check if it arrived
 *   on your PC
 */

#include <adafruit_feather.h>

#define WLAN_SSID             "yourSSID"
#define WLAN_PASS             "yourPassword"

// Your local PC's IP to test the throughput
// Run this command to create a server on your PC
// > nc -lu 8888
IPAddress server_ip(192, 168, 0, 20);
const uint16_t port = 8888;

AdafruitUDP udp;
const uint16_t local_port = 88;

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

  Serial.println("UDP Client Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  udp.begin(local_port);

  Serial.println("Please run the following command on your PC");
  Serial.printf("> nc -lu %d", port);
  Serial.println();
  Serial.println();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  char* input;

  // Prompt and get input from user
  Serial.print("Enter character(s) to send: ");
  input = getUserInput();
  Serial.println();

  // Send to server
  Serial.printf("Sending '%s' to ", input);
  Serial.print(server_ip);
  Serial.print(" ... ");

  udp.beginPacket(server_ip, port);
  udp.println(input); // send with newline
  udp.endPacket();

  Serial.println("OK");
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
    @brief  Get user input from Serial
*/
/**************************************************************************/
char* getUserInput(void)
{
  static char inputs[64+1];
  memset(inputs, 0, sizeof(inputs));

  // wait until data is available
  while( Serial.available() == 0 ) { delay(1); }

  uint8_t count=0;
  do
  {
    count += Serial.readBytes(inputs+count, 64);
  } while( (count < 64) && Serial.available() );

  return inputs;
}
