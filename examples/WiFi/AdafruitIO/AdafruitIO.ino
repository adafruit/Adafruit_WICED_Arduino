/*
  Adafruit IO - MQTT Example:
  1.  Connect to pre-specified AP
  2.  Set the LastWill message
  3.  Connect to Adafruit IO with user name & password
  4.  Publish random value in [-50,50] to specified feed on AIO every 20 seconds

  author: huynguyen
 */

#include "adafruit_wifi.h"
#include "itoa.h"

#define WLAN_SSID          "SSID of AP"
#define WLAN_PASS          "Password of AP"

#define MQTT_HOST          "io.adafruit.com"
#define MQTT_PORT          1883
#define CLIENT_ID          "Adafruit"
#define ADAFRUIT_USERNAME  "See your username at accounts.adafruit.com"
#define AIO_KEY            "Your AIO key"
#define FEED_PATH          ADAFRUIT_USERNAME "/feeds/feed-name"
#define LASTWILL_TOPIC     ADAFRUIT_USERNAME "/feeds/feed-status"
#define LASTWILL_MESSAGE   "Offline"
#define QOS                1
#define RETAIN             0


uint16_t wifi_error, mqtt_error, subs_error;
int temp;

/**************************************************************************/
/*!
    @brief  Connect to pre-specified AP

    @return Error code
*/
/**************************************************************************/
uint16_t connectAP()
{
  // Attempt to connect to an AP
  Serial.print(F("Attempting to connect to: "));
  Serial.println(WLAN_SSID);

  uint16_t error = wiced.connectAP(WLAN_SSID, WLAN_PASS);

  if (error == ERROR_NONE)
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
    @brief  Connect to pre-specified Broker

    @return Error code
*/
/**************************************************************************/
uint16_t connectBroker()
{
  // Attempt to connect to a Broker
  Serial.print(F("Attempting to connect to broker: "));
  Serial.print(MQTT_HOST); Serial.print(":"); Serial.println(MQTT_PORT);

  uint16_t error = wiced.mqttConnect(MQTT_HOST, MQTT_PORT, CLIENT_ID,
                                     ADAFRUIT_USERNAME, AIO_KEY);
  if (error == ERROR_NONE)
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
    @brief  Subscribe to a specified topic

    @return Error code
*/
/**************************************************************************/
uint16_t subscribeTopic()
{
  // Attempt to connect to a Broker
  Serial.print(F("Attempting to subscribe to the topic = <"));
  Serial.print(FEED_PATH); Serial.println(">");

  uint16_t error = wiced.mqttSubscribe(FEED_PATH, QOS);

  if (error == ERROR_NONE)
  {
    Serial.println(F("Succeeded!"));
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
  Serial.println(F("Adafruit IO - MQTT Example\r\n"));

  // If you want to use LED for debug
  pinMode(BOARD_LED_PIN, OUTPUT);

  wifi_error = connectAP();

  // Set LastWill message
  if (wiced.mqttLastWill(LASTWILL_TOPIC, LASTWILL_MESSAGE, QOS, RETAIN) == ERROR_NONE)
  {
    Serial.println(F("LastWill message has been set!\r\n"));
  }
  else
  {
    Serial.println(F("Fail to set LastWill message!\r\n"));
  }

  mqtt_error = connectBroker();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() {
  if (wifi_error == ERROR_NONE)
  {
    if (mqtt_error == ERROR_NONE)
    {
      // Buffer for temperature
      char str[4];
      temp = random(-50, 50);
      if (temp > 100) temp = 0;
      itoa(temp, str, 10);

      // qos = 1, retain = 0
      if (wiced.mqttPublish(FEED_PATH, str, QOS, RETAIN) == ERROR_NONE)
      {
        Serial.print(F("Published Message to ")); Serial.println(FEED_PATH);
        Serial.print(F("Value = ")); Serial.println(temp);
      }
      else
      {
        Serial.println(F("Publish Error!"));
      }
    }
    else
    {
      // Try to reconnect to MQTT Server
      mqtt_error = connectBroker();
    }
  }
  else
  {
    // Try to reconnect to AP
    wifi_error = connectAP();
  }

  Serial.println(F(""));
  togglePin(BOARD_LED_PIN);  // Toggle LED for debug
  delay(20000);              // Delay 20 seconds before next loop
}
