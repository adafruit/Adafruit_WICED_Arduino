/*
  Random Number Example using RNG HW

  author: huynguyen
 */

#include "adafruit_wifi.h"

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

  Serial.println(F("Random Number Example\r\n"));
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
  if (feather.randomNumber(&random32bit) == 0)
  {
    Serial.print(F("Generated random number: "));
    Serial.println(random32bit, DEC);
    Serial.println(F(""));
  }

  delay(10000);
}
