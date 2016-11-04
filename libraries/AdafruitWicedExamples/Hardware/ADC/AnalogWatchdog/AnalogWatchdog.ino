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

/*
 * This sketch uses AdafruitADC as analog watchdog which will fire 
 * a callback when volatge on analogPin is out of guarded threshold.
 * Guarded zone is 500-2000 mV (LOW_THRESHOLD, HIGH_THRESHOLD) due to
 * the fact that floating voltage is ~800-1000 mV.
 * 
 * When analogPin's voltage
 * - GND  (under threshold)  : LED is off
 * - 3.3V (higher threshold) : LED is on
 * - within threshold        : LED blinks
 */

#include <adafruit_feather.h>

/**
 * High & Low threshold in milli volatges
 */
#define HIGH_THRESHOLD    2000
#define LOW_THRESHOLD     500

int analogPin = PC5;
int ledPin = PA15;

// Initialized within guard zone
uint32_t violated_value = LOW_THRESHOLD+1;

// millisecond for blinking
uint32_t ms = 0;

/* Callback fires when volatge on analogPin is out of guarded threshold 
 * 
 * WARNING: adc_interrupt() is excecuted in ISR context which will
 * block all other threads (USB, network etc...). Running 
 * time-consuming code such Serial.println() will halt other 
 * subsystems. It is better to use a boolean value as flag and
 * run the handling code in loop()
 */
void adc_interrupt(void) 
{
  // Skip first few samples since it is not stable yet
  if (millis() > 1000)
  {
    violated_value = ADC.value();
  }
}

void setup() 
{
  pinMode(ledPin, OUTPUT);
  
  // Init ADC with analog pin
  ADC.begin(analogPin);

  // map 0-3.6V to 12 bits ADC
  uint16_t low  = map(LOW_THRESHOLD , 0, 3600, 0, 4095);
  uint16_t high = map(HIGH_THRESHOLD, 0, 3600, 0, 4095);

  // Set up threshold
  ADC.setThreshold(low, high);
  
  // Set up interrupt callback
  // Fired when the value is out of threshold
  ADC.attachInterrupt(adc_interrupt);

  // Set sample rate, possible value is in enum adc_smp_rate (adc.h)
  // When speed up the sample rate, you may need to increase
  // the buffer. Otherwise buffer overrun error will occur
  ADC.setSampleRate(ADC_SMPR_28_5);
  
  // Start ADC sampling
  ADC.start(ADC_MODE_WATCHDOG);
}

void loop() 
{
  if ( violated_value < LOW_THRESHOLD )
  {
    digitalWrite(ledPin, LOW);
  }else if (violated_value > HIGH_THRESHOLD)
  {
    digitalWrite(ledPin, HIGH);
  }else 
  {
    // Blink when within guarded zone
    // Check one per second
    if ( millis() > ms + 1000 )
    {   
      ms = millis(); 
      togglePin(ledPin);
    }    
  }
}

