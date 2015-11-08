/*
  HTTP Example:
  1.  Connect to pre-specified AP
  2.  Send GET/POST request to server

  author: huynguyen
 */

#include "adafruit_wifi.h"

#define WLAN_SSID            "SSID of AP"
#define WLAN_PASS            "Password of AP"

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
    @brief  Connect to pre-specified AP

    @return Error code
*/
/**************************************************************************/
int connectAP()
{
  // Attempt to connect to an AP
  Serial.print(F("Attempting to connect to: "));
  Serial.println(WLAN_SSID);

  int error = feather.connectAP(WLAN_SSID, WLAN_PASS);

  if (error == 0)
  {
    Serial.println(F("Connected!"));
  }
  else
  {
    Serial.print(F("Failed! Error: "));
    Serial.println(error, HEX);
  }
  Serial.println("");

  return error;
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  // If you want to use LED for debug
  pinMode(BOARD_LED_PIN, OUTPUT);
  
  // wait for Serial
  while (!Serial) delay(1);

  Serial.println(F("HTTP Example\r\n"));

  wifi_error = connectAP();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() {
  // put your main code here, to run repeatedly
  Serial.println(F("Toggle LED"));
  togglePin(BOARD_LED_PIN);
  
  if (wifi_error == 0)
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
