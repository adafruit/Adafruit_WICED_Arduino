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
#include "certificate_mosquitto.h"

/* This sketch demonstrates subscribe/unsubscribe activity with
 * callbacks.
 * 
 * It will connect to a public MQTT server (with/without TLS)
 * and subscribe to defined TOPIC_SUBSCRIBE.
 * 
 * - When a message is received, it will echo back at TOPIC_ECHO
 * - If the message is unsubscribed (via .unsubscribe), Feather
 *   will unsubscribe from TOPIC_SUBSCRIBE and won't be able to
 *   echo content back to the broker.
 * 
 * Note: TOPIC_SUBSCRIBE and TOPIC_ECHO must not be the same topic!
 * (e.g must not "adafruit/+" and "adafruit/echo"), since this will
 * cause and infinite loop (received -> echo -> received -> ....)
 *
 * For details on the MQTT broker server see http://test.mosquitto.org/
 *  - Port 1883 : MQTT, unencrypted
 *  - Port 8883 : MQTT, encrypted (TLS)
 *
 * Note: may You need an MQTT desktop client such as the lightweight
 * Java client included in this repo: org.eclipse.paho.mqtt.utility-1.0.0.jar
 * 
 * For information on configuring your system to work with MQTT see:
 * - https://learn.adafruit.com/desktop-mqtt-client-for-adafruit-io/installing-software
 *
 * To run this demo
 * 1. Change the SSID/PASS to match your access point
 * 2. Decide whether you want to use TLS/SSL or not (USE_TLS)
 * 3. Change CLIENTID, TOPIC, WILL_MESSAGE
 * 4. Compile and run
 * 5. Use MQTT desktop client to connect to the same sever and publish to
 *    any topic beginning with "adafruit/feather/" . To be able to recieve
 *    the echo message, please also subcribe to "adafruit/feather_echo".
 */

#define WLAN_SSID         "yourSSID"
#define WLAN_PASS         "yourPass"

#define USE_TLS           0

#define BROKER_HOST       "test.mosquitto.org"
#define BROKER_PORT       (USE_TLS ? 8883 : 1883)

// Uncomment to set your own ClientID, otherwise a random ClientID is used
//#define CLIENTID          "Adafruit Feather"

#define TOPIC_SUBSCRIBE   "adafruit/feather/+"
#define TOPIC_ECHO        "adafruit/feather_echo"

#define WILL_TOPIC        "adafruit/feather"
#define WILL_MESSAGE      "Goodbye!!"

AdafruitMQTT mqtt;

/**************************************************************************/
/*!
    @brief  Disconnect handler for MQTT broker connection
*/
/**************************************************************************/
void disconnect_callback(void)
{
  Serial.println();
  Serial.println("-----------------------------");
  Serial.println("DISCONNECTED FROM MQTT BROKER");
  Serial.println("-----------------------------");
  Serial.println();
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("MQTT Subscribe Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the MQTT client to auto print error codes and halt on errors
  mqtt.err_actions(true, true);

  // Set ClientID if defined
  #ifdef CLIENTID
  mqtt.clientID(CLIENTID);
  #endif

  // Last will must be set before connecting since it is part of the connection data
  mqtt.will(WILL_TOPIC, WILL_MESSAGE, MQTT_QOS_AT_LEAST_ONCE);

  // Set the disconnect callback handler
  mqtt.setDisconnectCallback(disconnect_callback);

  Serial.printf("Connecting to " BROKER_HOST " port %d ... ", BROKER_PORT);
  if (USE_TLS)
  {  
    // Disable default RootCA to save SRAM since we don't need to
    // access any other site except test.mosquitto.org
    Feather.useDefaultRootCA(false);

    // mosquitto CA is pre-generated using pycert.py
    Feather.addRootCA(rootca_certs, ROOTCA_CERTS_LEN);

    // Connect with SSL/TLS
    mqtt.connectSSL(BROKER_HOST, BROKER_PORT);
  }else
  {
    mqtt.connect(BROKER_HOST, BROKER_PORT);
  }
  Serial.println("OK");

  Serial.print("Subscribing to " TOPIC_SUBSCRIBE " ... ");
  mqtt.subscribe(TOPIC_SUBSCRIBE, MQTT_QOS_AT_MOST_ONCE, subscribed_callback); // Will halted if an error occurs
  Serial.println("OK");
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{

}

/**************************************************************************/
/*!
    @brief  Subscribe callback handler

    @param  topic_data  topic name's contents in byte array (not null terminated)
    @param  topic_len   topic name's length

    @param  mess_data   message's contents in byte array (not null terminated)
    @param  mess_len    message's length

    @note   'topic_data' and 'mess_data' are byte array without null-terminated
    like C-style string. Don't try to use Serial.print() directly, use the UTF8String
    datatype or Serial.write() instead.
*/
/**************************************************************************/
void subscribed_callback(char* topic_data, size_t topic_len, uint8_t* mess_data, size_t mess_len)
{
  // Use UTF8String class for easy printing of UTF8 data
  UTF8String utf8Topic(topic_data, topic_len);
  UTF8String utf8Message(mess_data, mess_len);

  // Print out topic name and message
  Serial.print("[Subscribed] ");
  Serial.print(utf8Topic);
  Serial.print(" : ") ;
  Serial.println(utf8Message);

  // Echo back
  mqtt.publish(TOPIC_ECHO, utf8Message); // Will halted if an error occurs

  // Unsubscribe from SUBSCRIBED_TOPIC2 if we received an "unsubscribe" message
  // Won't be able to echo anymore
  if ( utf8Message == "unsubscribe" )
  {
    Serial.print("Unsubscribing from " TOPIC_SUBSCRIBE " ... ");
    mqtt.unsubscribe(TOPIC_SUBSCRIBE); // Will halt if fails
    Serial.println("OK");
  }
}

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
  Serial.println();

  return Feather.connected();
}
