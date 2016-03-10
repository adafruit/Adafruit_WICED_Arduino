#include <Arduino.h>
#include <adafruit_crc32.h>

AdafruitCRC32 crc32;

#define TEXT_DATA   "wwww.adafruit.com"

void setup() 
{
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("CRC32 Example\r\n");

  Serial.print("CRC32 of " TEXT_DATA " : ");
  Serial.println(crc32.compute(TEXT_DATA));
}

void loop() 
{

}
