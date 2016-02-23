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

void test_message(const char* str, int pin)
{
  Serial.printf("Enter to set %s as %s", pinName(pinout), str);
  Serial.println();
  getUserInput();
}

void test_output(int pin)
{
  Serial.println("  - Set HIGH (Enter to continue)");
  digitalWrite(pin, HIGH);
  getUserInput();

  Serial.println("  - Set LOW (Enter to continue)");
  digitalWrite(pin, LOW) ;
  getUserInput();
}

void test_pwm(int pin)
{
  Serial.println("  - PWM from 0 to 65535 (Enter to continue)");
  while (!Serial.available())
  {
    for (int fadeValue = 0; fadeValue <= 65535; fadeValue += 1280) {
      // Sets the value (range from 0 to 65535):
      pwmWrite(pin, fadeValue);
      // Wait for 30 milliseconds to see the dimming effect:
      delay(30);
    }

    // Fade out from max to min in increments of 1280 points:
    for (int fadeValue = 65535 ; fadeValue >= 0; fadeValue -= 1280) {
      // Sets the value (range from 0 to 1280):
      pwmWrite(pin, fadeValue);
      // Wait for 30 milliseconds to see the dimming effect:
      delay(30);
    }
  }

  getUserInput();
}

// the loop function runs over and over again forever
void loop() 
{
  //--------------------------------------------------------------------+
  // OUTPUT TEST
  //--------------------------------------------------------------------+
  // OUTPUT Push Pull mode
  test_message("OUTPUT PUSH PULL", pinout);
  pinMode(pinout, OUTPUT);
  test_output(pinout);

  // OUTPUT Open Drain mode
  test_message("OUTPUT OPENDRAIN", pinout);
  pinMode(pinout, OUTPUT_OPEN_DRAIN);
  test_output(pinout);

  //--------------------------------------------------------------------+
  // PWM Test
  //--------------------------------------------------------------------+
  // PWM mode
  test_message("PWM", pinout);
  pinMode(pinout, PWM);
  test_pwm(pinout);

  // PWM Opendrain mode
  test_message("PWM OPENDRAIN", pinout);
  pinMode(pinout, PWM_OPEN_DRAIN);
  test_pwm(pinout);

  //--------------------------------------------------------------------+
  // INPUT Test
  //--------------------------------------------------------------------+
  // Input Floating (no pull)
  Serial.printf("Enter to set %s as INPUT FLOATING (no pull)", pinName(pinin));
  Serial.println();
  getUserInput();

  pinMode(pinin, INPUT);
  /* do something with input pin */

  // Input Pullup
  Serial.printf("Enter to set %s as INPUT PULLUP", pinName(pinin));
  Serial.println();
  getUserInput();

  pinMode(pinin, INPUT_PULLUP);
  /* do something with input pin */

  // Input Pulldown
  Serial.printf("Enter to set %s as INPUT PULLDOWN", pinName(pinin));
  Serial.println();
  getUserInput();

  pinMode(pinin, INPUT_PULLDOWN);
  /* do something with input pin */

  // Input Analog
  Serial.printf("Enter to set %s as INPUT ANALOG", pinName(pinin));
  Serial.println();
  getUserInput();

  pinMode(pinin, INPUT_ANALOG);
  /* do something with input pin */
  
  Serial.println();
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
    count += Serial.readBytes(inputs+count, 64);
  } while( (count < 64) && Serial.available() );

  return inputs;
}
