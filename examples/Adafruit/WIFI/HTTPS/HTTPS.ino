/*
  HTTPS Example:
  1.  Connect to pre-specified AP
  2.  Send GET/POST request to server

  Note: cert_to_h.py is used to generate "certificate.h" from certificate file (.pem)
  Usage:    python cert_to_h.py <cert_file> <variable_name> <output_filename>
  Example:  python cert_to_h.py google.pem root_ca_cert certificate

  Certificate file (*.pem) for a website is generated using openssl ultility.
  Usage:    echo "" | openssl s_client -connect <hostname>:<port> -showcerts \
            -CApath /etc/ssl/certs | awk '/BEGIN CERT/ {p=1}; p; /END CERT/ {p=0}' > <filename>.pem

  Example:  echo "" | openssl s_client -connect www.adafruit.com:443 -showcerts \
            -CApath /etc/ssl/certs | awk '/BEGIN CERT/ {p=1}; p; /END CERT/ {p=0}' > <filename>.pem

  author: huynguyen
 */

#include "adafruit_wifi.h"
#include "certificate.h"

#define WLAN_SSID            "thach"
#define WLAN_PASS            "hoangthach"

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
    int https_error = -1;
    if ( (https_error = feather.httpsRequest(URL, root_ca_cert, CONTENT, METHOD, BUFFER_LENGTH, result_buf) ) == 0)
    {
      Serial.println(F("Download Completed!\r\n"));
      Serial.println((char*)result_buf);
    }
    else
    {
      Serial.print(F("Error: "));
      Serial.println(https_error, HEX);
    }
  }

  delay(10000);
}
