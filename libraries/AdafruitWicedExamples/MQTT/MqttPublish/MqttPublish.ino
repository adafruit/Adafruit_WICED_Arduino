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

/* This sketch connects to a public MQTT server (with/without TLS)
 * and publishes a message to the specified topic every 5 seconds.
 *
 * - Server : test.mosquitto.org
 * - Port   : 1883 // MQTT - unencrypted
 * - Port   : 8883 // MQTT - encrypted (TLS)
 *
 * Note: may You need an MQTT desktop client such as
 * - The lightweight Java client included in this repo: org.eclipse.paho.mqtt.utility-1.0.0.jar or
 * - Full desktop client MQTT.fx https://learn.adafruit.com/desktop-mqtt-client-for-adafruit-io/installing-software
 *
 * To run this demo:
 * 1. Change the SSID/PASS to match your access point
 * 2. Decide whether you want to use TLS/SSL or not (USE_TLS)
 * 3. Change TOPIC, PUBLISH_MESSAGE, WILL_MESSAGE, enable CLIENTID if needed
 * 4. Compile and run
 * 5. Use an MQTT desktop client to connect to the same sever and
 *    subscribe to the defined topic to monitor the published
 *    message(s).
 */

#define WLAN_SSID         "yourSSID"
#define WLAN_PASS         "yourPass"

// Connect using TLS/SSL or not
#define USE_TLS           0

#define BROKER_HOST       "test.mosquitto.org"
#define BROKER_PORT       (USE_TLS ? 8883 : 1883 )

// Uncomment to set your own ClientID, otherwise a random ClientID is used
//#define CLIENTID          "Adafruit Feather"

#define TOPIC             "adafruit/feather"
#define PUBLISH_MESSAGE   "Hello from Adafruit WICED Feather"
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

  Serial.println("MQTT Publish Example\r\n");

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
  mqtt.will(TOPIC, WILL_MESSAGE, MQTT_QOS_AT_LEAST_ONCE);

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
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  Serial.print("Publishing to " TOPIC " ... ");
  mqtt.publish(TOPIC, PUBLISH_MESSAGE, MQTT_QOS_AT_LEAST_ONCE); // Will halted if an error occurs
  Serial.println("OK");

  delay(5000);
}

/**************************************************************************/
/*!
    @brief  Connect to defined Access Point
*/
/**************************************************************************/
bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Please wait while connecting to: '" WLAN_SSID "' ... ");
  
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
