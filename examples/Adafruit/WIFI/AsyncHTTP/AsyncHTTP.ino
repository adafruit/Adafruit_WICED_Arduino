/*
  AsyncHTTP Example:
  
  1.  Start the HTTP server via the python script in 'AsyncHTTP/http_server'
      $ cd http_server
      $ python server.py
  2.  Make a note of your local IP address (the method to determine your IP
      address will depend on your operating system)
  3.  Add your AP details via WLAN_SSID and WLAN_PASS in this sketch
  4.  Add the URL for the HTTP server you started to URL in this sketch,
      using the IP address of your machine and the filesname to load.
      For example:
      "192.168.1.1/text_1KB.txt"
  2.  Run the example and open serial monitor to see the async HTTP request

  author: huynguyen
 */

#include "adafruit_wifi.h"

#define WLAN_SSID            "SSID"
#define WLAN_PASS            "PASS"

#define URL                  "IP_Address_of_Server/text_1KB.txt"
#define CONTENT              ""
#define METHOD               GET_METHOD

int wifi_error = -1; // FAIL
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
    This function is called whenever new data received
*/
/**************************************************************************/
void rxCallback(uint8_t* data, uint16_t data_length, uint16_t available)
{
  Serial.print(F("Data Received!\r\n"));
  Serial.println((char*)data);
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
  
  feather.addHttpDataReceivedCallBack(rxCallback);
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
    if ( (http_error = feather.asyncHttpRequest(URL, CONTENT, METHOD) ) != 0)
    {
      Serial.print(F("Error: "));
      Serial.println(http_error, HEX);
    }
  }

  Serial.println(F("\r\n"));
  delay(10000);
}
