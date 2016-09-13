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

/* This sketch connects to a AWS IOT with mutual verification.
 * User enter '0' or '1' to update AWS shadow device as well as 
 * the LED on WICED module
 *
 * To run this demo:
 * 1. Create an account and log in http://aws.amazon.com/iot
 * 2. Create a THING and named it 'led'
 * 3. Select "led" thing and choose "Connect a Device", then select "Embeeded C" and click "Genereate Certificate and Policy"
 * 4. Download the generated certificate and private key to this sketch folder. Then click "Confirm & Start connecting"
 * 5. Then You could see these AWS_IOT_MQTT_HOST, AWS_IOT_MQTT_PORT, AWS_IOT_MQTT_CLIENT_ID, AWS_IOT_MY_THING_NAME, copy these 
 * and change those in this sketch. (You could always view this later in your resource management). 
 * 6. Copy the contents of private key to 'aws_private_key' variable.
 *    NOTE: Each line must be added with '\n' and quoted with " " to be parsed successfully.
 * 7. Using pycert to convert certificate to header file by running
 *      $ python ../../../tools/pycert/pycert.py convert -c local_cert -l LOCAL_CERT_LEN 214900e9fd-certificate.pem.crt.txt
 *    This should create an certificates.h with variable local_cert.
 * 8. Close and reopen this sketch to load certificates.h
 * 9. Compile and run
 */

#include "certificates.h"

#define WLAN_SSID         "yourSSID"
#define WLAN_PASS         "yourPass"

int ledPin = PA15;

AdafruitMQTT mqtt;

// ======================================================
#define AWS_IOT_MQTT_HOST              "A1B0KY2Z6UVZ7R.iot.us-west-2.amazonaws.com"
#define AWS_IOT_MQTT_PORT              8883
#define AWS_IOT_MQTT_CLIENT_ID         "led"
#define AWS_IOT_MY_THING_NAME          "led"
// ======================================================

#define AWS_IOT_MQTT_TOPIC             "$aws/things/" AWS_IOT_MY_THING_NAME "/shadow/update"

#define SHADOW_PUBLISH_STATE_OFF      "{ \"state\": {\"reported\": { \"status\": \"OFF\" } } }"
#define SHADOW_PUBLISH_STATE_ON       "{ \"state\": {\"reported\": { \"status\": \"ON\" } } }"

const char aws_private_key[] = 
"-----BEGIN RSA PRIVATE KEY-----\n"
"Your Key line 1 with\n"
"Quote each of your key's lines like this example\n"
"-----END RSA PRIVATE KEY-----";

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
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println("AWS IOT Example\r\n");

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

  // Set ClientID
  mqtt.clientID(AWS_IOT_MQTT_CLIENT_ID);

  // Set the disconnect callback handler
  mqtt.setDisconnectCallback(disconnect_callback);

  // default RootCA include certificate to verify AWS (
  Feather.useDefaultRootCA(true);

  // Setting Indentity with AWS Private Key & Certificate
  mqtt.tlsSetIdentity(aws_private_key, local_cert, LOCAL_CERT_LEN);

  // Connect with SSL/TLS
  Serial.printf("Connecting to " AWS_IOT_MQTT_HOST " port %d ... ", AWS_IOT_MQTT_PORT);
  mqtt.connectSSL(AWS_IOT_MQTT_HOST, AWS_IOT_MQTT_PORT);
  Serial.println("OK");

  Serial.print("Subscribing to " AWS_IOT_MQTT_TOPIC " ... ");
  mqtt.subscribe(AWS_IOT_MQTT_TOPIC, MQTT_QOS_AT_MOST_ONCE, subscribed_callback); // Will halted if an error occurs
  Serial.println("OK");
  
  // Message to user
  Serial.print("Enter '0' or '1' to update feed: ");
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  // Get input from user '0' to off, '1' to update feed
  if ( Serial.available() )
  {
    char c = Serial.read();

    // echo
    Serial.println(c);
    Serial.print("Enter '0' or '1' to update feed: ");

    //Serial.print("Publishing to " AWS_IOT_MQTT_TOPIC " ... ");
    if ( c == '0' )
    {
      mqtt.publish(AWS_IOT_MQTT_TOPIC, SHADOW_PUBLISH_STATE_OFF, MQTT_QOS_AT_LEAST_ONCE); // Will halted if an error occurs
    }else if ( c == '1' )
    {
      mqtt.publish(AWS_IOT_MQTT_TOPIC, SHADOW_PUBLISH_STATE_ON, MQTT_QOS_AT_LEAST_ONCE); // Will halted if an error occurs
    }else
    {
      // do nothing
    }
  }
  
  delay(5000);
}

/**************************************************************************/
/*!
    @brief  MQTT subscribe event callback handler

    @param  topic      The topic causing this callback to fire
    @param  message    The new value associated with 'topic'

    @note   'topic' and 'message' are UTF8Strings (byte array), which means
            they are not null-terminated like C-style strings. You can
            access its data and len using .data & .len, although there is
            also a Serial.print override to handle UTF8String data types.
*/
/**************************************************************************/
void subscribed_callback(UTF8String topic, UTF8String message)
{
  if ( 0 == memcmp(SHADOW_PUBLISH_STATE_OFF, message.data, message.len) )
  {
    digitalWrite(ledPin, LOW);
  }
  
  if ( 0 == memcmp(SHADOW_PUBLISH_STATE_ON, message.data, message.len) )
  {
    digitalWrite(ledPin, HIGH);
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
