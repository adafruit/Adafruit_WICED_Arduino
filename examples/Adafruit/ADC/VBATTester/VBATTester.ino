/*
  VBAT ADC Measurement

  Reads the VBAT voltage level off of the optional voltage divider on 
  pin A1/ADC1.  You need to solder closed SJ1 on the bottom of the
  WICED Feather PCBs to enable routing the VBAT level through the
  voltage divider into ADC1.

  STM32F205 ADCs are 12-bit for a range of 0..4095, which gives us
  0.80566 mV per LSB with a 3.30V supply/vref.
  
  A 4.2V LIPO running through the 10K+10K voltage divider will give
  2.1V or an approximate ADC value of 2606 (2100 * 1.24121212).
  
  Assuming a LIPO can go from as low as 3.2V completely drained to
  4.2V fully charged, and adding a small margin of error we get:

  LIPO Power (3.2-4.25V) = 1985 - 2637

  When no LIPO is attached, the output of the LIPO charger will
  output ~4.3V, which means any value > ~2650 on the ADC likely
  means there is no LIPO battery connected and we are running from
  USB power.

  created 18 Dec. 2015
  by K. Townsend (KTOWN)
*/

const int analogInPin = PA1; // The pin where the voltage divider is set

int   vbatADC   = 0;         // The raw ADC value off the voltage div
float vbatLSB   = 0.80566F;  // mV per LSB to convert raw values to volts
float vbatFloat = 0.0F;      // The ADC equivalent in millivolts

void setup() 
{
  pinMode(analogInPin, INPUT_ANALOG);
  Serial.begin(115200);

  // Wait for the Serial Monitor to open
  // (Disable this check to run without Serial Monitor)
  while (!Serial) { yield(); }
}

void loop() 
{
  // Read the analog in value:
  vbatADC = analogRead(analogInPin);

  // Multiply the ADC by mV per LSB, and then
  // double the output to compensate for the
  // 10K+10K voltage divider
  vbatFloat = ((float)vbatADC * vbatLSB) * 2.0F;

  // Print the results to the serial monitor:
  Serial.print("VBAT = " );
  Serial.print(vbatFloat/1000, 4); // Adjust to display in volts
  Serial.print("V (");
  Serial.print(vbatADC);
  Serial.print(") - ");

  // Display the power source
  if (vbatADC > 2650)
  {
    Serial.println("USB Power");
  }
  else
  {
    Serial.println("LIPO Power");
  }

  // Wait a bit before the next sample
  delay(500);
}

