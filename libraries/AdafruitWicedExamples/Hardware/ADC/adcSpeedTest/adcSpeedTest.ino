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
 * This sketch uses AdafruitADCto test the speed of WICED ADC which is up to
 * 2 MSPS for single channel or 4 MSPS for dual, 6 MSPS for triple channel.
 * 
 * To run this example we need an oscilloscope to accurately measure the
 * frequency of ledPin's toggle, which occurs each time the FIFO is full. Multiply
 * with the BUFSIZE to got the overal speed of ADC sampling. e.g
 * - BUFSIZE = 128
 * - Frequency of ledPins is measured at 15.4 khz
 * --> sampling speed is 128 * 15.4 ~ 1971.2 KSPS ~ 1.9 MSPS
 * 
 * reference link on measuring using scope 
 * http://nicecircuits.com/playing-with-analog-to-digital-converter-on-arduino-due/
 */

#include <adafruit_feather.h>

/* Known Issues Warnings:
 * Faster sample rate needs a bigger buffer to prevent buffer overrun, which will
 * suspend the ADC sampling. Following is tested max sample rate by buffer size
 * (subject to change in future release)
 *  - bufsize = 128 --> max sample is 1.5
 *  - bufsize = 64  --> max sample is 7.5
 *  - bufsize = 32  --> max sample is 13.5
 *  - bufsize = 16  --> max sample is 28.5
 *  - bufsize = 8   --> max sample is 71.5
 *  - bufsize = 4   --> max sample is 239.5
 *  - bufsize < 4   --> cannot work
 */
#define BUFSIZE   128

int analogPin = PC5;
int ledPin = PA15;
uint16_t buffer[BUFSIZE] = { 0 };


/* Callback fires when buffer is full of samples. Sampling is 
 * paused when this function is executed until ADC.resume() 
 * is called (should be after processing ADC samples).
 * 
 * WARNING: adc_interrupt() is excecuted in ISR context which will
 * block all other threads (USB, network etc...). Running 
 * time-consuming code such Serial.println() will halt other 
 * subsystems. It is better to use a boolean value as flag and
 * run the handling code in loop()
 */
void adc_interrupt(void) 
{
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin, LOW);

  // Continue sampling
  ADC.resume();
}

void setup() 
{
  pinMode(ledPin, OUTPUT);

  // Init ADC with analog pin
  ADC.begin(analogPin);

  // Set up FIFO holding ADC data
  ADC.setBuffer(buffer, BUFSIZE);
  
  // Set up interrupt callback
  // Fired when the FIFO is full of samples
  ADC.attachInterrupt(adc_interrupt);

  // Set sample rate, possible value is in enum adc_smp_rate (adc.h)
  // When speed up the sample rate, you may need to increase
  // the buffer. Otherwise buffer overrun error will occur
  ADC.setSampleRate(ADC_SMPR_1_5);
  
  // Start ADC sampling
  ADC.start();
}

void loop() 
{
}

