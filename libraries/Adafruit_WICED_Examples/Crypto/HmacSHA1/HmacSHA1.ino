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

#include <Arduino.h>
#include "adafruit_sha1.h"

/**
   Result can be verified by comparing with online generator such as
   http://www.freeformatter.com/hmac-generator.html
*/


#define HMAC_SHA1_KEY     "Adafruit"
#define HMAC_SHA1_INPUT   "WICED Feather"

AdafruitSHA1 sha1;

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

  Serial.println("HMAC SHA1 Example\r\n");

  Serial.println("HMAC SHA1 Key   : " HMAC_SHA1_KEY);
  Serial.println("HMAC SHA1 Input : " HMAC_SHA1_INPUT);

  uint8_t hmac_result[20];
  sha1.generateHMAC(HMAC_SHA1_KEY, HMAC_SHA1_INPUT, hmac_result);

  Serial.println();
  Serial.print("HMAC SHA1 Result: " );
  Serial.printBuffer(hmac_result, 20);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  togglePin(PA15);
  delay(1000);
}


