/*
  Adafruit IO - MQTT Example
  This sketch publishes random values in [0,50] to specified topic on Adafruit IO every 20 seconds.

  1.  Add your AP details via WLAN_SSID and WLAN_PASS in this sketch

  2.  Add the MQTT host & MQTT port (default is 1883)

  3.  Add your adafruit user name and AIO key and default clientID
      Note: The 23-character random client ID can be generated using
            mqttGenerateRandomID function

  4.  Add your publish topic, subscribed topic, and lastwill configuration

  5.  Run the example and open serial monitor to see the result

  author: huynguyen
 */

#include "adafruit_feather.h"
#include "itoa.h"

#define WLAN_SSID            "YOUR SSID"
#define WLAN_PASS            "YOUR PASSWORD"

#define MQTT_HOST            "io.adafruit.com"
#define MQTT_PORT            1883

#define CLIENT_ID            "Adafruit"
#define ADAFRUIT_USERNAME    "See your username at accounts.adafruit.com"
#define AIO_KEY              "Your AIO key"
#define PUBLISH_TOPIC        ADAFRUIT_USERNAME "/feeds/temp"
#define SUBSCRIBE_TOPIC      ADAFRUIT_USERNAME "/#"

#define LASTWILL_ENABLED     1
#define CONNECTED_TOPIC      ADAFRUIT_USERNAME "/feeds/status"
#define CONNECTED_MESSAGE    "Online"
#define LASTWILL_TOPIC       ADAFRUIT_USERNAME "/feeds/status"
#define LASTWILL_MESSAGE     "Offline"
#define QOS                  1
#define RETAIN               0


int wifi_error  = -1; // FAIL
int mqtt_error  = -1; // FAIL
int subs_error  = -1; // FAIL
int isConnected = 0;

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

  int error = feather.mqttConnect(MQTT_HOST, MQTT_PORT, clientID, ADAFRUIT_USERNAME, AIO_KEY); 
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
    @brief This function is called whenever a new event occurs
*/
/**************************************************************************/
void mqttCallback(mqtt_evt_opcode_t event, uint16_t len, uint8_t* data)
{
  switch (event)
  {
    case MQTT_EVT_DISCONNECTED:
      Serial.println(F("Disconnected\r\n"));
      isConnected = 0;
      break;
    case MQTT_EVT_TOPIC_CHANGED:
      Serial.println(F("Topic changed"));
      // Print the message
      // [0:7]: UTC time & date (64-bit integer)
      // [8: ]: "topic=value"
      for (int i = 8; i < len; i++)
      {
        Serial.write(data[i]);
      }
      Serial.println(F("\r\n"));
      break;
    default:
      break;
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

  // Register the mqtt callback handler
  feather.addMqttCallBack(mqttCallback);

  // Connect to broker
  mqtt_error = connectBroker();
  if (mqtt_error == 0)
  {
    isConnected = 1;
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
    if (isConnected)
    {
      // Buffer for temperature
      char str[3] = "0";
      uint32_t randomNum = 0;
      if (feather.randomNumber(&randomNum) == 0)
      {
        randomNum = randomNum % 50;
        utoa( randomNum, str, 10);
      }

      // qos = 1, retain = 0
      if (feather.mqttPublish(PUBLISH_TOPIC, str, QOS, RETAIN) == 0)
      {
        Serial.print(F("Published Message to ")); Serial.println(PUBLISH_TOPIC);
        Serial.print(F("Value = ")); Serial.println(randomNum);
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
