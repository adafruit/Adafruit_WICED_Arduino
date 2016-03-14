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

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() 
{
  Serial.print("Bootloader version      : ");
  Serial.println(Feather.bootloaderVersion());

  Serial.print("SDK version             : ");
  Serial.println(Feather.sdkVersion());
  
  Serial.print("Firmware version        : ");
  Serial.println(Feather.firmwareVersion());

  Serial.print("Arduino library version : ");
  Serial.println(Feather.arduinoVersion());
  
  Serial.println();
  Serial.println("Print again in 10 seconds");
  delay(10000);
}
