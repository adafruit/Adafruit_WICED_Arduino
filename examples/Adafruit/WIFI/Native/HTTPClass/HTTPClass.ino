/*
  AdafruitHTTP Class Example:
  This example demonstrates the use of AdafruitHTTP Class to send HTTP request

  1. Add your AP details via WLAN_SSID and WLAN_PASS in this sketch

  2. Add the URL of the server, request content & HTTP method

  3. Set attributes for AdafruitHTTP class in setup() function:
     - Set timeout
     - Register the async callback handler
     - Init Root CA Certificate Chain (used when TLS enabled)
     - Enable/disable TLS

  4. Run the example and open serial monitor to see the response

  Notes: The number of callbacks depends on the implementation of each server.
  In the callback function, all of these following operations can be performed:

  -  Extract the response status code
        int responseCode;
        responseCode = adaHttp.getResponseCode();
        Serial.println(responseCode, DEC);

  -  Extract the header values
        char contentLength[8];
        if (adaHttp.extractHeaderValue("Content-Length", contentLength) == 0)
        {
          Serial.print(F("Content-Length: "));
          Serial.println(contentLength);
        }

  -  Print the entire header by checking endOfHeaderReached()
        while (!adaHttp.endOfHeaderReached())
        {
          char c = adaHttp.read();
          Serial.write(c);
        }

  -  Or skip the response header
        adaHttp.skipHeader();

  -  Print the entire response to serial monitor
        while (adaHttp.available())
        {
          char c = adaHttp.read();
          Serial.write(c);
        }

  author: huynguyen
 */

#include "adafruit_http.h"
#include "certificate.h"

#define WLAN_SSID            "SSID"
#define WLAN_PASS            "PASSWORD"

#define URL                  "github.com"
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
    @brief This function is called whenever a new packet is received due to
           the adaHttp.sendRequest function
*/
/**************************************************************************/
int receive_callback(void* arg1, void* arg2)
{
  (void) arg1; // reserve for future
  (void) arg2; // reserve for future

  Serial.println("\r\nReceive callback");

  int responseCode;
  if ( (responseCode = adaHttp.getResponseCode()) > 0)
  {
    Serial.print(F("Response code: "));
    Serial.println(responseCode, DEC);
  }

  // Extract "Content-type" header value
  char contentType[30];
  if (adaHttp.extractHeaderValue("Content-Type", contentType) == 0)
  {
    Serial.print(F("Content-Type: "));
    Serial.println(contentType);
  }

  // Extract "Content-length" header value
  char contentLength[8];
  if (adaHttp.extractHeaderValue("Content-length", contentLength) == 0)
  {
    Serial.print(F("Content-length: "));
    Serial.println(contentLength);
  }

  // Skip the response header
  adaHttp.skipHeader();

  // Read the content of response
  while (adaHttp.available())
  {
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
  //Initialize serial and wait for port to open:
  while (!Serial)
  {
    // wait for serial port to connect. Needed for native USB port only
    delay(1);
  }

  Serial.println(F("Adafruit HTTP Class Example\r\n"));

  // Register the async callback handler
  adaHttp.setReceivedCallback(receive_callback);

  // Init Root CA Certificate Chain
  adaHttp.initCACerts(ca_cert);

  // Enable TLS
  adaHttp.enableTLS();

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
  delay(30000);
}
