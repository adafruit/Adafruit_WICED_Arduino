/*
  Starts/Stop AP mode on the module, causing the module to advertise a new 
  wireless network and SSID, etc.
  
  author: huynguyen 
 */

#include "adafruit_feather.h"

static bool defaultMode = 0;
/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board    
*/
/**************************************************************************/
void setup()
{
  pinMode(BOARD_LED_PIN, OUTPUT);
  // wait for Serial
  while (!Serial) delay(1); 
  
  // initialize serial port for input and output
//  Serial.begin(11500);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  if (defaultMode == 1)
  {
    defaultMode = 0;
    
    // Start AP mode with default SSID and PASSWORD
    if (Feather.startAP() == ERROR_NONE)
    {
      Serial.println("AP mode started with default SSID and PASSWORD");
    }
    else
      Serial.println("AP mode error!");
  }
  else
  {
    defaultMode = 1;
    
    // Start AP mode with default SSID and PASSWORD
    char* ssid = "ADAFRUIT";
    char* pass = "12345678";
    if (Feather.startAP(ssid, pass) == ERROR_NONE)
    {
      Serial.print("AP mode started with SSID = "); Serial.print(ssid);
      Serial.print(" and PASSWORD = "); Serial.println(pass);
    }
    else
      Serial.println("AP mode error!");
  }
  
  Serial.println("Waiting for 30 seconds before stopping the AP mode");
  delay(30000);
  
  // Stop AP mode
  if (Feather.stopAP() == ERROR_NONE)
  {
    Serial.println("AP mode stopped");
  }
  else
    Serial.println("Error while stop AP mode!");

  delay(1000);
   
  togglePin(BOARD_LED_PIN);
  delay(5000);    // Delay 10 seconds before next loop
}
