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

#define WLAN_SSID       "thach"
#define WLAN_PASS       "thach367"

// Public Broker description http://test.mosquitto.org/
// 1883 : MQTT, unencrypted
// 8883 : MQTT, encrypted
#define BROKER_HOST     "test.mosquitto.org"
#define BROKER_PORT     1883

#define CLIENTID        "Adafruit Feather"
#define TOPIC           "adafruit/feather"

#define WILL_MESSAGE    "Good Bye!!"
#define PUBLISH_MESSAGE "Hello from Adafruit WICED Feather"



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

  Serial.printf("Connecting to " BROKER_HOST " port %d ... ", BROKER_PORT);
  if ( !mqtt.connect(BROKER_HOST, BROKER_PORT) )
  {
    Serial.printf("Failed! %s (%d)", mqtt.errstr(), mqtt.errno());
    while(1) delay(1);
  }
  Serial.println("OK");

  Serial.print("Publishing to " TOPIC " ... ");
  if ( !mqtt.publish(TOPIC, PUBLISH_MESSAGE) )
  {
    Serial.printf("Failed! %s (%d)", mqtt.errstr(), mqtt.errno());
    while(1) delay(1);
  }
  Serial.println("OK");

  mqtt.disconnect();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
}
