/*
  Adafruit HTTP Class Example

  author: huynguyen
 */

#include "adafruit_http.h"

#define WLAN_SSID            "SSID"
#define WLAN_PASS            "PASSWORD"

#define URL                  "IPADDRESS:8000/text_100B.txt"
#define CONTENT              ""
#define METHOD               GET_METHOD


int           wifi_error  = -1;    // FAIL
unsigned long crc         = ~0L;
int           data_found  = 0;
AdafruitHTTP  adaHttp;

/**************************************************************************/
/*!
    @brief  Connect to the WLAN_SSID access point using WLAN_PASS

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

int receive_callback(void* arg1, void* arg2)
{
  (void) arg1; // reserve for future
  (void) arg2; // reserve for future
  
  Serial.println("receive callback");
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (adaHttp.available()) {
    char c = adaHttp.read();
    Serial.write(c);
  }
  
  return 0;
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

  Serial.println(F("Adafruit HTTP Class Example\r\n"));

  // Register the async callback handler
  adaHttp.setReceivedCallback(receive_callback);

//  adaHttp.enableTLS();

  // Try to connect to the access point
  wifi_error = connectAP();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() 
{
  // put your main code here, to run repeatedly
  Serial.println(F("Toggle LED"));
  togglePin(BOARD_LED_PIN);
   
  if (wifi_error == 0)
  {
    int http_error;
    if ( (http_error = adaHttp.sendRequest(URL, CONTENT, METHOD) ) != 0)
    {
      Serial.print(F("Error: "));
      Serial.println(http_error, HEX);
    }
  }

  Serial.println(F("\r\n"));
  delay(10000);
}
