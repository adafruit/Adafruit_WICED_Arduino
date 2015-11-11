/*
  Adafruit IO - MQTT Example:
  1.  Connect to pre-specified AP
  2.  Set the LastWill message
  3.  Generate a 23-character random client ID
  3.  Connect to Adafruit IO with user name, password, client ID & TLS config (optional)
  4.  Read arrived message from ASYN FIFO
  5.  Publish random value in [0,50] to specified topic on AIO every 20 seconds

  author: huynguyen
 */

#include "adafruit_wifi.h"
#include "itoa.h"

#define WLAN_SSID            "SSID of AP"
#define WLAN_PASS            "Password of AP"

#define MQTT_HOST            "io.adafruit.com"
#define MQTT_PORT            8883
#define TLS_ENABLED          1

#define CLIENT_ID            "Adafruit"
#define ADAFRUIT_USERNAME    "See your username at accounts.adafruit.com"
#define AIO_KEY              "Your AIO key"
#define PUBLISH_TOPIC        ADAFRUIT_USERNAME "/feeds/temp"
#define SUBSCRIBE_TOPIC      ADAFRUIT_USERNAME "/feeds/temp"

#define LASTWILL_ENABLED     1
#define CONNECTED_TOPIC      ADAFRUIT_USERNAME "/feeds/status"
#define CONNECTED_MESSAGE    "Online"
#define LASTWILL_TOPIC       ADAFRUIT_USERNAME "/feeds/status"
#define LASTWILL_MESSAGE     "Offline"
#define QOS                  1
#define RETAIN               0

#define MAX_LENGTH_MESSAGE   1024


int wifi_error = -1; // FAIL
int mqtt_error = -1; // FAIL
int subs_error = -1; // FAIL
int temp;

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
    @brief  Connect to pre-specified Broker

    @return Error code
*/
/**************************************************************************/
int connectBroker()
{
  // Generate a 23-character random clientID
  char* clientID = NULL;
  char id[24];
  if (feather.mqttGenerateRandomID(id, 23) == 0)
  {
    clientID = id;
  }
  else
  {
    clientID = CLIENT_ID;
  }

  // Attempt to connect to a Broker
  Serial.print(F("Attempting to connect to broker: "));
  Serial.print(MQTT_HOST); Serial.print(":"); Serial.println(MQTT_PORT);

  int error = feather.mqttConnect(MQTT_HOST, MQTT_PORT, clientID, ADAFRUIT_USERNAME, AIO_KEY, TLS_ENABLED); 
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
    @brief  Subscribe to a specified topic

    @return Error code
*/
/**************************************************************************/
int subscribeTopic()
{
  // Attempt to connect to a Broker
  Serial.print(F("Attempting to subscribe to the topic = <"));
  Serial.print(SUBSCRIBE_TOPIC); Serial.println(">");

  int error = feather.mqttSubscribe(SUBSCRIBE_TOPIC, QOS);

  if (error == 0)
  {
    Serial.println(F("Succeeded!"));
  }
  else
  {
    Serial.print(F("Failed! Error: "));
    Serial.println(error, HEX);
  }
  Serial.println(F(""));

  return error;
}

/**************************************************************************/
/*!
    @brief  Read message from the ASYNC FIFO

    @return NONE
*/
/**************************************************************************/
void readArrivedMessage()
{
  if (subs_error == 0)
  {
    uint16_t irq_error;

    // Specify the current number of items in the ASYNC FIFO
    uint16_t n_item;
    irq_error = feather.irqCount(&n_item);
    if (irq_error == 0)
    {
      if (n_item > 0)
      {
        Serial.print(n_item);
        Serial.println(F(" message(s) found in ASYNC FIFO"));
        uint8_t response[MAX_LENGTH_MESSAGE];
        uint16_t response_len;
        for (int n = 0; n < n_item; n++)
        {
          // Read message from subscribed topic
          irq_error = feather.irqRead(&response_len, response);
          if (irq_error == 0)
          {
            if (response_len > 0)
            {
              Serial.print(F("Message read! "));
              // [0:1]: interrupt source
              // [2:9]: UTC time & date (64-bit integer)
              // [10:]: Value
              for (int i = 10; i < response_len; i++)
              {
                Serial.write(response[i]);
              }
              Serial.println(F(""));
            }
          }
          else
          {
            Serial.print(F("Read Failed! Error: "));
            Serial.println(irq_error, HEX);
          }
        }
      }
      else
      {
        Serial.println(F("No arrived message!"));
      }
    }
    else
    {
      Serial.print(F("Failed to specify the current number of items in the ASYNC FIFO! Error: "));
      Serial.println(irq_error, HEX);
    }

    Serial.println("");
  }
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

  Serial.println(F("Adafruit IO - MQTT Example\r\n"));

  wifi_error = connectAP();

  if (LASTWILL_ENABLED == 1)
  {
    // Set LastWill topic + message
    if (feather.mqttLastWill(false, LASTWILL_TOPIC, LASTWILL_MESSAGE, QOS, RETAIN) == 0)
    {
      Serial.println(F("LastWill message has been set!\r\n"));
    }
    else
    {
      Serial.println(F("Fail to set LastWill message!\r\n"));
    }
  
    // Set LastWill Connected topic + message
    if (feather.mqttLastWill(true, CONNECTED_TOPIC, CONNECTED_MESSAGE, QOS, RETAIN) == 0)
    {
      Serial.println(F("Connected topic & message have been set!\r\n"));
    }
    else
    {
      Serial.println(F("Fail to set Connected topic & message!\r\n"));
    }
  }

  mqtt_error = connectBroker();
  if (mqtt_error == 0)
  {
    subs_error = subscribeTopic();
  }
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() {
  if (wifi_error == 0)
  {
    if (mqtt_error == 0)
    {
      readArrivedMessage();

      // Buffer for temperature
      char str[3] = "0";
      uint32_t randomNum;
      if (feather.randomNumber(&randomNum) == 0)
      {
        utoa( randomNum % 50, str, 10);
      }

      // qos = 1, retain = 0
      if (feather.mqttPublish(PUBLISH_TOPIC, str, QOS, RETAIN) == 0)
      {
        Serial.print(F("Published Message to ")); Serial.println(PUBLISH_TOPIC);
        Serial.print(F("Value = ")); Serial.println(temp);
      }
      else
      {
        Serial.println(F("Publish Error!"));
      }
    }
  }

  Serial.println(F(""));
  togglePin(BOARD_LED_PIN);  // Toggle LED for debug
  delay(20000);              // Delay 20 seconds before next loop
}
