int irqpin = PC5;
int ledpin = PB5;

volatile int ledstate = LOW;

void setup() 
{
  // Setup the LED pin as an output
  pinMode( ledpin, OUTPUT );
  // Setup the IRQ pin as an input (pulled high)
  pinMode( irqpin, INPUT_PULLUP );
  // Attach 'blink' as the interrupt handler when IRQ pin changes
  attachInterrupt( irqpin, blink, CHANGE );
}

void loop() 
{
  // Set the LED to the current led state
  digitalWrite(ledpin, ledstate);
  yield();
}

void blink() 
{
  ledstate = !ledstate;
}
