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

// the setup function runs once when you press reset or power the board
void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);
}

// the loop function runs over and over again forever
void loop() 
{
  Serial.print("Bootloader version      : ");
  Serial.println(feather.bootloaderVersion());

  Serial.print("SDK version             : ");
  Serial.println(feather.sdkVersion());
  
  Serial.print("Firmware version        : ");
  Serial.println(feather.firmwareVersion());

  Serial.print("Arduino library version : ");
  Serial.println(feather.arduinoVersion());
  
  Serial.println();
  Serial.println("Print again in 10 seconds");
  delay(10000);
}
