/*
  This example will attempt to connect to a WiFi access point, then read
  the response from the 'x-www-form-urlencoded' URI as an HTTP POST
  operation. It will then display the response and disconnect from the AP.

  author: huynguyen 
 */

#include "adafruit_wifi.h"

/**************************************************************************/
/*!
    @brief  The setup function (runs once when the board resets or boots)
*/
/**************************************************************************/
void setup()
{
  pinMode(BOARD_LED_PIN, OUTPUT);
  
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
  // ToDo: Change the values below!
  char* ssid = "YOUR SSID";
  char* pass = "YOUR PASSWORD";
  if ( (error = feather.connectAP(ssid, pass) ) == ERROR_NONE)
  {
    Serial.print("Connected to AP with SSID = ");
    Serial.print(ssid);
    Serial.print(" and PASSWORD = ");
    Serial.println(pass);

    uint8_t response[1024];
    uint16_t buffer_len;

    char* get_uri = "http://www.adafruit.com/testwifi/index.html";
    Serial.println("Response from HTTP GET URI");
    buffer_len = sizeof(response);
    error = feather.httpGetUri(get_uri, &buffer_len, response);
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
    buffer_len = sizeof(response);
    if (feather.httpPost(post_uri, &buffer_len, response) == ERROR_NONE)
    {
      for (int i = 0; i < buffer_len; i++)
        Serial.write(response[i]);
    }
    else
      Serial.println("HTTP POST Error!");

    Serial.println("\r\n");

    feather.disconnect();
    Serial.println("Disconnected from AP");
  }
  else
    Serial.println("Connect Error!");

  Serial.println("");
  togglePin(BOARD_LED_PIN);
  delay(10000);   // Delay 10 seconds before next loop
}
