/*********************************************************************
 This is an example for our Feather WIFI modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <adafruit_feather.h>
#include <adafruit_mqtt.h>

#define WLAN_SSID         "yourSSID"
#define WLAN_PASS         "yourPASS"

// Public Broker: http://test.mosquitto.org/
// 1883 : MQTT, unencrypted
// 8883 : MQTT, encrypted

#define BROKER_HOST       "test.mosquitto.org"
#define BROKER_PORT       1883

#define CLIENTID          "Adafruit Feather"

#define TOPIC             "adafruit/feather"
#define SUBSCRIBED_TOPIC  "adafruit/+"  // All topics below 'adafruit/'
#define SUBSCRIBED_TOPIC2 "adafruit/test"

#define WILL_MESSAGE      "Good Bye!!"
#define PUBLISH_MESSAGE   "Hello from Adafruit WICED Feather"

AdafruitMQTT mqtt(CLIENTID);

/**************************************************************************/
/*!
    @brief  Connect to a specific access point
*/
/**************************************************************************/
bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);

  if ( Feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errno());
    Serial.println();
  }
  Serial.println("");

  return Feather.connected();
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Optional: Wait for USB serial to connect
  while (!Serial) delay(1);
  Serial.println("MQTT Test Example\r\n");

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }
  
  // Tell the MQTT client to auto print error codes and halt on errors
  mqtt.err_actions(true, true);

  // Last will must be set before connecting since it is part of the connection data
  mqtt.will(TOPIC, WILL_MESSAGE, MQTT_QOS_AT_LEAST_ONCE);

  Serial.printf("Connecting to " BROKER_HOST " port %d ... ", BROKER_PORT);
  mqtt.connect(BROKER_HOST, BROKER_PORT); 
  Serial.println("OK");

  Serial.print("Publishing to " TOPIC " ... ");
  mqtt.publish(TOPIC, PUBLISH_MESSAGE); // Will halted if an error occurs
  Serial.println("OK");

  Serial.print("Subscribing to " SUBSCRIBED_TOPIC " ... ");
  mqtt.subscribe(SUBSCRIBED_TOPIC, MQTT_QOS_AT_MOST_ONCE, subscribed_callback); // Will halted if an error occurs
  Serial.println("OK");

  Serial.print("Subscribing to " SUBSCRIBED_TOPIC2 " ... ");
  mqtt.subscribe(SUBSCRIBED_TOPIC2, MQTT_QOS_AT_MOST_ONCE, subscribed2_callback); // Will halted if an error occurs
  Serial.println("OK");

  // To test the last will message, disconnect power and wait for the keep alive interval to pass 
  // (default is 60 seconds)

  //Serial.print("Disconnect from Broker");
  //mqtt.disconnect();
}

/**************************************************************************/
/*!
    @brief  Subscribe callback handler
    @note   'topic' and 'message' are UTF8 strings (len+data), not
            null-terminated C string. Don't try to use Serial.print()
            directly. Use Serial.write() instead or the UTF8String
            datatype.
*/
/**************************************************************************/
void subscribed_callback(char* topic_data, size_t topic_len, uint8_t* mess_data, size_t mess_len)
{
  // Use UTF8String class for easy printing of UTF8 data
  UTF8String utf8Topic(topic_data, topic_len);
  UTF8String utf8Message(mess_data, mess_len);
  
  Serial.print("[Subscribed1] ");
  Serial.print(utf8Topic);
  Serial.print(" : ") ;
  Serial.println(utf8Message);
}

/**************************************************************************/
/*!
    @brief  Subscribe callback handler
    @note   'topic' and 'message' are UTF8 strings (len+data), not
            null-terminated C string. Don't try to use Serial.print()
            directly. Use Serial.write() instead or the UTF8String
            datatype.
*/
/**************************************************************************/
void subscribed2_callback(char* topic_data, size_t topic_len, uint8_t* mess_data, size_t mess_len)
{
  // Use UTF8String class for easy printing of UTF8 data
  UTF8String utf8Topic(topic_data, topic_len);
  UTF8String utf8Message(mess_data, mess_len);
  
  Serial.print("[Subscribed2] ");
  Serial.print(utf8Topic);
  Serial.print(" : ") ;
  Serial.println(utf8Message);

  // Unsubscribe from SUBSCRIBED_TOPIC2 if we received an "unsubscribe" message
  if ( utf8Message == "unsubscribe" )
  {
    Serial.print("Unsubscribing to " SUBSCRIBED_TOPIC2 " ... ");
    mqtt.unsubscribe(SUBSCRIBED_TOPIC2); // Will halt if fails
    Serial.println("OK");
  }
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
}
