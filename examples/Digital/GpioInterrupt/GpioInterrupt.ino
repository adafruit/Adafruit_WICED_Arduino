int pin = PB3;
int led = PB5;
volatile int state = LOW;

void setup() {
    pinMode(led, OUTPUT);
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(pin, blink, CHANGE);
}

void loop() {
    digitalWrite(led, state);
}

void blink() {
    state = !state;
}
