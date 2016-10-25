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

int ledPin = PA15;

void setup() 
{
  // Configure ledPin in PWM mode (optional)
  pinMode(ledPin, PWM);
  
	Serial.begin(115200);
  
  // Wait for the Serial Monitor to open
  while (!Serial)
  {
    /* Delay required to avoid RTOS task switching problems */
    delay(1);
  }
  
  Serial.println("PWM Period Example (duty cycle = 50%)");
  Serial.print("Enter Period value in microseconds: ");
}

void loop() 
{
  // Check if data has been sent from the computer:
  if (Serial.available()) 
  {
    char* input = getUserInput();
    Serial.println(input);
    int n = atoi(input);

    pwmPeriod(ledPin, n);    
    analogWrite(ledPin, 127); // need to re-write after Period changes

    Serial.print("Enter Period value in microseconds: ");
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
