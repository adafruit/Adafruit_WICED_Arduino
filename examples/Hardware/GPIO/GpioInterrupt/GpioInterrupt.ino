int irqpin = PC5;
int ledpin = BOARD_LED_PIN;

volatile int ledstate = LOW;

void setup() 
{
  // Setup the LED pin as an output
  pinMode( ledpin, OUTPUT );
  // Setup the IRQ pin as an input (pulled high)
  pinMode( irqpin, INPUT_PULLUP );
  // Attach 'blink' as the interrupt handler when IRQ pin changes
  // Note: Can be set to RISING, FALLING or CHANGE
  attachInterrupt( irqpin, blink, CHANGE );
}

void loop() 
{
  // Set the LED to the current led state
  digitalWrite(ledpin, ledstate);
}

void blink() 
{
  ledstate = !ledstate;
}
