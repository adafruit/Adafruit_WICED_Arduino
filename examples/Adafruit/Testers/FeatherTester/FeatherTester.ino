/*
  WICED Feather Tester

  Tests every pin on the Adafruit WICED Feather board
  
  created 28 Dec. 2015
  by K. Townsend (KTOWN)
*/

//#include <Adafruit_GPS.h>
//Adafruit_GPS GPS(&Serial1);

/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the Serial Monitor to open
  while (!Serial) { yield(); }

  // Display the test suite selection menu
  display_menu();
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void display_menu()
{
  delay(500);  // Short delay for cosmetic reasons
  Serial.println("");
  Serial.println("WICED Feather Tester");
  Serial.println("-------------------------------------------------------------------------------");
  Serial.println("Select a test to run:");
  Serial.println("");
  Serial.println("[1]  - ADC Input Test");
  Serial.println("[2]  - GPS Test - UART1");
  Serial.println("");
  Serial.println("Enter you selection in the Serial Monitor and press <enter>");
  Serial.println("");
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void run_adc_test()
{
  float       vbatLSB        = 0.80566F;  // mV per LSB
  const int   analogPins[8]  = { WAKE, PC3, PC2, PA3, PA2, PA1, PC5, PA4 };
  const char* analogNames[8] = { "WAKE", "C3  ", "C2  ", "A3  ", "A2  ", "A1  ", "C5  ", "A4  " };
  const int   count          = sizeof(analogPins)/sizeof(analogPins[0]);

  // Set all ADC pins to analog input mode
  uint8_t i;
  for (i = 0; i < count; i++)
  {
    pinMode(analogPins[i], INPUT_ANALOG);
  }

  Serial.println("ADC Input Test");
  Serial.println("-------------------------------------------------------------------------------");
  Serial.println("This sketch reads all ADC pins and displays the raw 12-bit ADC value as well as");
  Serial.println("the  equivalent value in volts.");
  Serial.println("");
  Serial.println("Assuming a 3.3V ADC reference and an ADC range of 0..4095:");
  Serial.println("");
  Serial.println("1 ADC value = 0.80566406mV (3300mv / 4096)");
  Serial.println("");

  // Read the analog in values:
  for (i = 0; i < count; i++)
  {
    int   vbati = 0;    // Raw integer value
    vbati = analogRead(analogPins[i]);

    // Display the value in volts (based on 3.3V VRef)
    float vbatf = ((float)vbati * vbatLSB);
    Serial.print(analogNames[i]);
    Serial.print(" : ");
    Serial.print(vbatf/1000, 2);
    Serial.print("V");

    // Display the 12-bit raw ADC value
    Serial.print(" - ");
    if (vbati < 10) Serial.print("   ");
    if (vbati < 100) Serial.print("  ");
    if (vbati < 1000) Serial.print(" ");
    Serial.print(vbati);
    Serial.println(" / 4095");
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void run_gps_test()
{
  Serial.println("GPS Test");
  Serial.println("-------------------------------------------------------------------------------");
  Serial.println("This sketch reads GPS data from Serial1 and displays it in the Serial Monitor");
  Serial.println("");
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void loop()
{
  String user_input = "";
  int selection = 0;

  // Wait for user feedback, then parse feedback one byte at a time
  while((Serial.peek() != 255) && !selection)
  {
    char incoming = Serial.read();
    if (isdigit(incoming))
    {
      // Append the current digit to the string placeholder
      user_input += (char)incoming;
    }
    // Parse the string on new-line
    if (incoming == '\n')
    {
      selection = user_input.toInt();
    }
    delay(2);
  }


  // Run the appropriate test suite if we have a number
  if (selection)
  {
    switch(selection)
    {
      case 1: 
        run_adc_test();
        break;
      case 2:
        run_gps_test();
        break;
      default:
        Serial.print("Invalid selection: ");
        Serial.println(selection);
        Serial.println("");
        break;
    }

    // Display the main menu again
    display_menu();
  }
}

