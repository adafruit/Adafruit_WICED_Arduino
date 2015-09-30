/*
  Adafruit IO Example:
  1.  Connect to pre-specified AP
  2.  Set the LastWill message
  3.  Connect to Adafruit IO with username & password
  4.  Publish temperature value to specified feed on AIO every 15 seconds

  author: huynguyen 
 */

#include "adafruit_wifi.h"
#include "itoa.h"

uint16_t wifi_error, mqtt_error, subs_error;
uint16_t temp = 0;

/**************************************************************************/
/*!
    @brief  Connect to pre-specified AP
    
    @return Error code  
*/
/**************************************************************************/
uint16_t connectAP()
{
  // Connect to an AP
  char* ssid = "huy-laptop";
  char* pass = "12345678";
  uint16_t error = wiced.connectAP(ssid, pass);
  if (error == ERROR_NONE)
  {
    Serial.print("Connected to AP with SSID = ");
    Serial.print(ssid);
    Serial.print(" and PASSWORD = ");
    Serial.println(pass);
  }
  else
  {
    Serial.println("Connect AP Error!");
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
  char* host = "io.adafruit.com";
  char* clientID = "Adafruit";
  uint16_t port = 1883;
  char* username = "huynguyen";
  char* password = "2d0030ff67ee2abd40c2a1b3b0d7b5456df1739d";
  uint16_t error = wiced.mqttConnect(host, port, clientID, username, password);
  if (error == ERROR_NONE)
  {
    Serial.print("Connected to Broker = ");
    Serial.println(host);
  }
  else
  {
    Serial.println("Connect Broker Error!");
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
  char* topic = "huynguyen/feeds/temp";
  uint16_t error = wiced.mqttSubscribe(topic, 0);
  if (error == ERROR_NONE)
  {
    Serial.print("Subscribed to the topic = <");
    Serial.print(topic); Serial.println(">");
  }
  else
  {
    Serial.println("Subscribe Error!");
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
  pinMode(BOARD_LED_PIN, OUTPUT);
  delay(1000);

  wifi_error = connectAP();
  mqtt_error = connectBroker();

  // Set LastWill message
  if (wiced.mqttLastWill("huynguyen/feeds/temp","Offline",1,0) == ERROR_NONE)
  {
    Serial.println("Specified LastWill Message!");
  }
  
  // initialize serial port for input and output
//  Serial.begin(11500);
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
      // Publish a Message to a Topic
      char* topic = "huynguyen/feeds/temp";
      char str[4];
      temp = temp + 5;
      if (temp > 100) temp = 0;
      utoa(temp, str, 10);
      
      // qos = 1, retain = 0
      if (wiced.mqttPublish(topic, str, 0, 0) == ERROR_NONE)
      {
        Serial.print("Published Message! ");
        Serial.print("Value = ");
        Serial.println(temp);
      }
    }
    else
    {
      // Try to reconnect to MQTT Server    
      Serial.println("Trying to reconnect to MQTT Server...");
      mqtt_error = connectBroker();
    }
  }
  else
  {
    // Try to reconnect to AP
    Serial.println("Trying to reconnect to AP...");
    wifi_error = connectAP();
  }

  Serial.println("");
  togglePin(BOARD_LED_PIN);
  delay(15000);   // Delay 10 seconds before next loop
}
