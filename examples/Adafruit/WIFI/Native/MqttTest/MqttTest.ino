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

#define WLAN_SSID       "yourSSID"
#define WLAN_PASS       "yourPASS"

// Public Broker description http://test.mosquitto.org/
// 1883 : MQTT, unencrypted
// 8883 : MQTT, encrypted
#define BROKER_HOST       "test.mosquitto.org"
#define BROKER_PORT       1883

#define CLIENTID          "Adafruit Feather"

#define TOPIC             "adafruit/feather"
#define SUBSCRIBED_TOPIC  "adafruit/*"

#define WILL_MESSAGE      "Good Bye!!"
#define PUBLISH_MESSAGE   "Hello from Adafruit WICED Feather"


// Public user/pass are not required
AdafruitMQTT mqtt(CLIENTID);

/**************************************************************************/
/*!
    @brief  Connect to defined Access Point
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
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // wait for Serial
  while (!Serial) delay(1);
  Serial.println("MQTT Test Example\r\n");

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }
  
  // MQTT to auto print out error code & halted
  mqtt.erractions(true, true);

  // Will must be set before connect, since it is part of connect data
  mqtt.will(TOPIC, WILL_MESSAGE, MQTT_QOS_AT_LEAST_ONCE);

  Serial.printf("Connecting to " BROKER_HOST " port %d ... ", BROKER_PORT);
  mqtt.connect(BROKER_HOST, BROKER_PORT); 
  Serial.println("OK");

  Serial.print("Publishing to " TOPIC " ... ");
  mqtt.publish(TOPIC, PUBLISH_MESSAGE); // halted if failed
  Serial.println("OK");

  Serial.print("Suscribing to " SUBSCRIBED_TOPIC " ... ");
  mqtt.subscribe(SUBSCRIBED_TOPIC, MQTT_QOS_AT_MOST_ONCE, subscribed_callback); // halted if failed
  Serial.println("OK");

  // To test Will message, pull out power and wait for Keep Alive interval to pass 
  // (default is 60 seconds)

  //Serial.print("Disconnect from Broker");
  //mqtt.disconnect();
}

/**************************************************************************/
/*!
    @brief  Subscribed callback
    @note message is array of bytes, not null-terminated C string, don't 
    try to use Serial.print(), use Serial.write() instead
*/
/**************************************************************************/
void subscribed_callback(char* topic, uint8_t* message, size_t len)
{
  Serial.printf("[Subscribed] %s : ");
  Serial.write(message, len);
  Serial.println();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
}
