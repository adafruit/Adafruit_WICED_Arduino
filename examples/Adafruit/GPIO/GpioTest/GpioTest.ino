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

#include <adafruit_wifi.h>

int pinout = PA15;
int pinin  = PC5;

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(115200);

  // Wait for serial port to connect
  while(!Serial) delay(1);

  Serial.println("Gpio Test Example");
}

// the loop function runs over and over again forever
void loop() 
{
  //--------------------------------------------------------------------+
  // OUTPUT TEST
  //--------------------------------------------------------------------+
  // OUTPUT Push Pull mode
  Serial.printf("Send any keys to set %s as OUTPUT PUSH PULL", pinName(pinout));
  getUserInput();
  Serial.println();

  pinMode(pinout, OUTPUT);

  digitalWrite(pinout, HIGH); delay(1000);
  digitalWrite(pinout, LOW) ; delay(1000);

  // OUTPUT Open Drain mode
  Serial.printf("Send any keys to set %s as OUTPUT OPEN DRAIN", pinName(pinout));
  getUserInput();
  Serial.println();

  pinMode(pinout, OUTPUT_OPEN_DRAIN);

  digitalWrite(pinout, HIGH); delay(1000);
  digitalWrite(pinout, LOW) ; delay(1000);

  //--------------------------------------------------------------------+
  // PWM Test
  //--------------------------------------------------------------------+
  // PWM mode
  Serial.printf("Send any keys to set %s as PWM", pinName(pinout));
  getUserInput();
  Serial.println();

  pinMode(pinout, PWM);

  // PWM Opendrain mode
  Serial.printf("Send any keys to set %s as PWM OPEN DRAIN", pinName(pinout));
  getUserInput();
  Serial.println();

  pinMode(pinout, PWM_OPEN_DRAIN);

  //--------------------------------------------------------------------+
  // INPUT Test
  //--------------------------------------------------------------------+
  // Input Floating (no pull)
  Serial.printf("Send any keys to set %s as INPUT FLOATING (no pull)", pinName(pinin));
  getUserInput();
  Serial.println();

  pinMode(pinin, INPUT);
  /* do something with input pin */

  // Input Pullup
  Serial.printf("Send any keys to set %s as INPUT PULLUP", pinName(pinin));
  getUserInput();
  Serial.println();

  pinMode(pinin, PULLUP);
  /* do something with input pin */

  // Input Pulldown
  Serial.printf("Send any keys to set %s as INPUT PULLDOWN", pinName(pinin));
  getUserInput();
  Serial.println();

  pinMode(pinin, PULLDOWN);
  /* do something with input pin */
}

char* getUserInput(void)
{
  static char inputs[64+1];
  memset(inputs, 0, sizeof(inputs));

  // wait until data is available
  while( Serial.available() == 0 ) { delay(1); }

  uint8_t count=0;
  do
  {
    count += Serial.readBytes(inputs+count, maxSize);
  } while( (count < maxSize) && Serial.available() );

  return inputs;
}
