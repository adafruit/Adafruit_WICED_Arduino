/*
  AdafruitHTTP Class Example:

  author: huynguyen
 */

#include "adafruit_http.h"

#define WLAN_SSID            "SSID"
#define WLAN_PASS            "PASSWORD"

#define URL                  "IPADDRESS:8000/text_1KB.txt"
#define CONTENT              ""
#define METHOD               GET_METHOD


int           wifi_error  = -1;    // FAIL
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

/**************************************************************************/
/*!
    @brief This function is called whenever new data is received from
           the feather.asyncHttpRequest function
*/
/**************************************************************************/
int receive_callback(void* arg)
{
  (void) arg; // reserve for future
  
  Serial.println("\r\nReceive callback");

  int responseCode;
  char contentLength[8];
  char contentType[12];
  switch(adaHttp.getState())
  {
    case REQUEST_SENT:
      responseCode = adaHttp.getResponseCode();
      Serial.print(F("Response code: "));
      Serial.println(responseCode, DEC);
      break;

    case RESPONSE_CODE_READ:
      adaHttp.extractHeader("Content-type", contentType);
      Serial.print(F("Content-type: "));
      Serial.println(contentType);

      adaHttp.extractHeader("Content-Length", contentLength);
      Serial.print(F("Content-Length: "));
      Serial.println(contentLength);

      // Skip the HTTP header
      adaHttp.skipHeader();

      // Note: No 'break' command since the content can be in
      // the same packet with the header

    case HEADER_PASSED:
      // Read the content of response
      while (adaHttp.available())
      {
        char c = adaHttp.read();
        Serial.write(c);
      }
      break;
    default:
      break;
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
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    // wait for serial port to connect. Needed for native USB port only
    delay(1);
  }

  Serial.println(F("Adafruit HTTP Class Example\r\n"));

  // Register the async callback handler
  adaHttp.setReceivedCallback(receive_callback);

  // Enable TLS
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
