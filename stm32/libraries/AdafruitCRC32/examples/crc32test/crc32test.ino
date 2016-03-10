#include <Arduino.h>
#include <adafruit_crc32.h>

/* Run the crc32.py script to check the result. Change the TEXT_DATA
 * in the script to the same here
 */

AdafruitCRC32 crc32;

#define TEXT_DATA   "wwww.adafruit.com"

void setup() 
{
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("CRC32 Example\r\n");

  Serial.print("CRC32 of '" TEXT_DATA "' : ");
  Serial.println(crc32.compute(TEXT_DATA));
}

void loop() 
{

}
