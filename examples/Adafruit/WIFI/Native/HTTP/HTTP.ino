/*
  HTTP Example:
  1.  Connect to pre-specified AP
  2.  Send GET/POST request to server

  author: huynguyen
 */

#include "adafruit_feather.h"

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

#define BUFFER_LENGTH        (2048)

#define URL                 "www.adafruit.com/testwifi/index.html"
#define CONTENT             ""
#define METHOD              GET_METHOD

//#define URL                 "www.adafruit.com/testwifi/testpost.php"
//#define CONTENT             "name=foo&email=bar@adafruit.com"
//#define METHOD              POST_METHOD

//#define URL                 "www.google.com"
//#define CONTENT             ""
//#define METHOD              GET_METHOD


int wifi_error = -1; // FAIL
uint8_t result_buf[BUFFER_LENGTH];

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  // Setup LED for blinking
  pinMode(BOARD_LED_PIN, OUTPUT);
  
  // wait for Serial
  while (!Serial) delay(1);

  Serial.println(F("HTTP Example\r\n"));

  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);

  if ( feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println(F("Connected!"));
  }
  else
  {
    Serial.print(F("Failed! Error: "));
    Serial.println(feather.errno(), HEX);
  }
  Serial.println("");
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  togglePin(BOARD_LED_PIN);
  
  if ( feather.connected() )
  {
    int http_error = -1;
    if ( (http_error = feather.httpRequest(URL, CONTENT, METHOD, BUFFER_LENGTH, result_buf) ) == 0)
    {
      Serial.println(F("Download Completed!\r\n"));
      Serial.println((char*)result_buf);
    }
    else
    {
      Serial.print(F("Error: "));
      Serial.println(http_error, HEX);
    }
  }

  Serial.println(F("\r\n"));
  delay(10000);
}
