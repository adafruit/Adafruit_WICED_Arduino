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

#define PWM_FREQUENCY 10000 // 10khz

/*
 * Example use PA1, PA2, PA3 from the same PWM Timer (TIMER5).
 */
 
int redPin   = PA1;
int greenPin = PA2;
int bluePin  = PA3;

void setup() 
{
  // Configure ledPin in PWM mode: (optional)
  pinMode(redPin, PWM);
  pinMode(greenPin, PWM);
  pinMode(bluePin, PWM);

  // You don't need to set Frequency 3 times if they are from 
  // the same TIMER. Example just want to cover all pin configuration
  pwmFrequency(redPin, PWM_FREQUENCY);
  pwmFrequency(greenPin, PWM_FREQUENCY);
  pwmFrequency(bluePin, PWM_FREQUENCY);

  // start up with 50% each color
  analogWrite(redPin  , 128);
  analogWrite(greenPin, 128);
  analogWrite(bluePin , 128);
  
	Serial.begin(115200);
  
  // Wait for the Serial Monitor to open
  while (!Serial)
  {
    /* Delay required to avoid RTOS task switching problems */
    delay(1);
  }
  
  Serial.println("PWM RGB Example");
  Serial.print("Enter space separated RGB value e.g '50 100 200' : ");
}

void loop() 
{
  // Check if data has been sent from the computer:
  if (Serial.available()) 
  {
    char* input = getUserInput();
    Serial.println(input);
    
    int r = strtoul(input, &input, 0);
    int g = strtoul(input, &input, 0);
    int b = strtoul(input, &input, 0);

    analogWrite(redPin  , r);
    analogWrite(greenPin, g);
    analogWrite(bluePin , b);

    Serial.println();
    Serial.print("Enter space separated RGB value e.g '50 100 200' : ");
  }
}

/**************************************************************************/
/*!
    @brief  Get user input from Serial
*/
/**************************************************************************/
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
