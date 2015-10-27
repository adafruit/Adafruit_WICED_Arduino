/*
  HTTPS Example:
  1.  Connect to pre-specified AP
  2.  Get data from website: www.google.com with simple query

  Note: cert_to_h.py is used to generate "certificate.h" from certificate file (.pem)
  Usage:    python cert_to_h.py <variable_name> <cert_file>
  Example:  python cert_to_h.py root_ca_cert google_cert.pem

  author: huynguyen
 */

#include "adafruit_wifi.h"
#include "certificate.h"

#define WLAN_SSID            "huy-laptop"
#define WLAN_PASS            "12345678"
#define TLS_ENABLED          (1)

#define HOST_ADDRESS         "www.google.com"
#define BUFFER_LENGTH        (1024)

const char* query =
    "GET / HTTP/1.1\r\n"
    "Host: \r\n"
    "Connection: close\r\n"
    "\r\n";

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

  Serial.println(F("HTTPS Example\r\n"));

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
    if (feather.httpsGet(HOST_ADDRESS, root_ca_cert, query, BUFFER_LENGTH, result_buf) == 0)
    {
      Serial.println(F("Download Completed!\r\n"));
      Serial.println((char*)result_buf);
      Serial.println(F("\r\n"));
    }
  }

  delay(10000);
}
