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

/* This example tests the speed of TCP transfers from the feather to
 * your local PC. To run this example:
 * - Change the SSID/Password
 * - Change server_ip to your PC's IPv4 address
 * - Run 'nc -l 8888' to start a TCP server on your PC
 * - Compile and run the sketch
 */

#include <adafruit_feather.h>
#include <adafruit_tcp.h>

#define WLAN_SSID             "YOURSSID"
#define WLAN_PASS             "YOURPASS"

// Run this command to create a server on your PC
// > nc -l 8888
IPAddress server_ip(192, 168, 1, 100);
const uint16_t port = 8888;

AdafruitTCP tcp;

// Each member has 50 chars
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

  Serial.println("Throughput Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  Serial.print("\nStarting connection to ");
  Serial.print(server_ip);
  Serial.printf(":%d\r\n", port);

  tcp.usePacketBuffering(true); // use packet buffering for maximum performance
  tcp.connect(server_ip, port);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  // If the server isn't connected, stop the tcp:
  if (!tcp.connected())
  {
    Serial.println("The TCP server is unavailable or disconnected.");
    tcp.stop();

    // do nothing forever
    while (true) {
      delay(1);
    }
  }

  uint32_t start, stop, sent_loop;
  uint32_t remaining_loop = 20000; // number of loop to send, each loop is testStepLen
  start = stop = sent_loop = 0;

  Serial.println("Enter any key to start sending data");
  (void) getUserInput();

  start = millis();
  while (remaining_loop > 0)
  {
    tcp.write( testData[sent_loop%10] );

    sent_loop++;
    remaining_loop--;
  }

  // Data is buffered and only be sent when network packet is full
  // or flush() is called to optimize network usage
  tcp.flush();

  // print result
  stop = millis() - start;
  Serial.printf("Sent %d bytes in %.02f seconds.\r\n", sent_loop*testStepLen, stop/1000.0F);
  Serial.printf("Throughput = %f KB/s.\r\n", (float)(sent_loop*testStepLen) / stop);

  // if there are incoming bytes available
  // from the server, read them and print them:
  while ( tcp.available() )
  {
    char c = tcp.read();
    Serial.write(c);
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
