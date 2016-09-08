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
#include "adafruit_spiflash.h"

/* This example will erase SPI Flash chip to blank
 */

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(115200);

  // Wait for the USB serial to connect. Needed for native USB port only.
  while (!Serial) delay(1);
    
  Serial.println("SPI Flash Chip Erase Example");

  Serial.print("Erasing (~12s) ....");
  SpiFlash.eraseAll();
  Serial.println("Done");
}

// the loop function runs over and over again forever
void loop() 
{
  toggleLED();
  delay(1000);
}

