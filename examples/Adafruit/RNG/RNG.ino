/*
  Random Number Generator (RNG) Example
  1. Generate a 32-bit random number
  2. Generate a 23-character random ID

  author: huynguyen
 */

#include "adafruit_feather.h"

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup() {
  // If you want to use LED for debug
  pinMode(BOARD_LED_PIN, OUTPUT);

  // wait for Serial
  while (!Serial) delay(1);

  Serial.println(F("Random Number Generator Example\r\n"));
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() {
  // put your main code here, to run repeatedly
  Serial.println(F("Toggle LED"));
  togglePin(BOARD_LED_PIN);

  uint32_t random32bit;
  if (Feather.randomNumber(&random32bit) == 0)
  {
    Serial.print(F("Generated a random number: "));
    Serial.println(random32bit, DEC);
  }
  else
  {
    Serial.println(F("Failed to generate a random number!"));
  }

  char clientID[24];
  if (Feather.mqttGenerateRandomID(clientID, 23) == 0)
  {
    Serial.print(F("Generated a 23-character ID: "));
    Serial.println(clientID);
  }
  else
  {
    Serial.println(F("Failed to generate a random ID!"));
  }
  
  Serial.println(F(""));
  delay(10000);
}
