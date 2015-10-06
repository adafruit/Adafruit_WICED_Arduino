/*
  Connect to a WiFi access point. If success, retrieve the results of a 
  specified URI and send the 'x-www-form-urlencoded' URL as a HTTP POST 
  operation. Then finally disconnect from the WiFi AP.

  author: huynguyen 
 */

#include "adafruit_wifi.h"

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board    
*/
/**************************************************************************/
void setup()
{
  pinMode(BOARD_LED_PIN, OUTPUT);
  
  // initialize serial port for input and output
//  Serial.begin(11500);

  // wait for Serial
  while (!Serial) delay(1);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() {
  uint16_t error;
  // Connect to an AP
  char* ssid = "huy-laptop";
  char* pass = "12345678";
  if ( (error = wiced.connectAP(ssid, pass) ) == ERROR_NONE)
  {
    Serial.print("Connected to AP with SSID = ");
    Serial.print(ssid);
    Serial.print(" and PASSWORD = ");
    Serial.println(pass);

    uint16_t buffer_len;
    uint8_t response[1024];
    char* get_uri = "http://www.adafruit.com/testwifi/index.html";
    Serial.println("Response from HTTP GET URI");
    error = wiced.httpGetUri(get_uri, &buffer_len, response);
    if (error == ERROR_NONE)
    {
      Serial.println("");
      for (int i = 0; i < buffer_len; i++)
        Serial.write(response[i]);
    }
    else
    {
      Serial.print("HTTP GET Error: ");
      Serial.println(error, HEX);
    }

    Serial.println("\r\n");

    char* post_uri = "http://www.adafruit.com/testwifi/testpost.php?name=foo&email=bar@adafruit.com";
    Serial.println("Response from HTTP POST");
    Serial.println("");
    if (wiced.httpPost(post_uri, &buffer_len, response) == ERROR_NONE)
    {
      for (int i = 0; i < buffer_len; i++)
        Serial.write(response[i]);
    }
    else
      Serial.println("HTTP POST Error!");

    Serial.println("\r\n");

    // Stop AP mode
    if (wiced.disconnectAP() == ERROR_NONE)
    {
      Serial.println("Disconnected from AP");
    }
    else
      Serial.println("Disconnect Error!");
  }
  else
    Serial.println("Connect Error!");

  Serial.println("");
  togglePin(BOARD_LED_PIN);
  delay(10000);   // Delay 10 seconds before next loop
}
