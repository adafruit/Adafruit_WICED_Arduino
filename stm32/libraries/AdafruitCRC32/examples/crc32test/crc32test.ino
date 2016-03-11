#include <Arduino.h>
#include <adafruit_crc32.h>

/* Run the crc32.py script to check the result. Change the TEXT1 & TEXT3
 * in the script to the same here
 */

AdafruitCRC32 crc32;

#define TEXT1   "Adafruit"
#define TEST2   " Industries"

void setup() 
{
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("CRC32 Example\r\n");

  
  Serial.print("CRC32 1 piece : ");
  Serial.println(crc32.compute(TEXT1 TEST2));

  // need to reset to re-compute
  crc32.reset();
  crc32.compute(TEXT1);
  crc32.compute(TEST2);
  
  Serial.print("CRC32 2 piece : ");
  Serial.println(crc32.crc);
}

void loop() 
{

}
