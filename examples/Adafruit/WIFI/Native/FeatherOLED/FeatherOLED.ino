#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <Adafruit_FeatherOLED_WiFi.h>
#include <Adafruit_Sensor.h>
#include <adafruit_wifi.h>
#include <itoa.h>

#define WLAN_SSID                 "SSID"
#define WLAN_PASS                 "PASSWORD"
#define WLAN_SECURITY             ENC_TYPE_AUTO

#define VBAT_ENABLED              1
#define VBAT_PIN                  PA1

#define SENSOR_TSL2561_ENABLED    0
#if SENSOR_TSL2561_ENABLED
  #include <Adafruit_TSL2561_U.h>
  bool _tslFound = false;
  Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
#endif

#define MQTT_ENABLED              0
#define MQTT_HOST                 "io.adafruit.com"
#define MQTT_PORT                 1883
#define MQTT_AIOUSERNAME          "AIOUSERNAME"
#define MQTT_AIOKEY               "AIOKEY"
#define MQTT_QOS                  1
#define MQTT_RETAIN               0
#define MQTT_TOPIC_VBAT           MQTT_AIOUSERNAME "/feeds/vbat"
#define MQTT_TOPIC_TSL2561_LUX    MQTT_AIOUSERNAME "/feeds/lux"

Adafruit_FeatherOLED_WiFi oled = Adafruit_FeatherOLED_WiFi();

bool mqtt_connected = false;

/**************************************************************************/
/*!
    @brief  Connect to the AP
    @return Error code
*/
/**************************************************************************/
bool connectAP()
{   
  oled.refreshIcons();
  oled.clearMsgArea();
  oled.println("Connecting to ...");
  oled.println(WLAN_SSID);
  oled.display();

  // Attempt to connect to the AP
  if ( feather.connect(WLAN_SSID, WLAN_PASS, WLAN_SECURITY ) )
  {
    int8_t rssi = feather.RSSI();
    uint32_t ipAddress = feather.localIP();
    oled.setConnected(true);
    oled.setRSSI(rssi);
    oled.setIPAddress(ipAddress);
    oled.refreshIcons();
    oled.clearMsgArea();
  }
  else
  {
    // Display the error message
    err_t err = feather.errno();
    oled.setConnected(false);
    oled.refreshIcons();
    oled.clearMsgArea();
    oled.println("Connection Error:");
    switch (err)
    {
      case ERROR_WWD_ACCESS_POINT_NOT_FOUND:
        // SSID wasn't found when scanning for APs
        oled.println("Invalid SSID");
        break;
      case ERROR_WWD_INVALID_KEY:
        // Invalid SSID passkey
        oled.println("Invalid Password");
        break;
      default:
        // The most likely cause of errors at this point is that
        // you are just out of the device/AP operating range
        oled.print(feather.errno());
        oled.print(":");
        oled.println(feather.errstr());
        oled.refreshIcons(); // Refresh icons in case the text ran over
        break;
    }    
    oled.display();
    // Return false to indicate that we received an error (available in feather.errno)
    return false;
  }

  return true;
}

#if MQTT_ENABLED
/**************************************************************************/
/*!
    @brief  Connect to pre-specified Broker

    @return Error code
*/
/**************************************************************************/
int connectBroker()
{
  // Generate a random 23-character client ID
  char _clientID[24];
  memset(_clientID, 0, 24);
  feather.mqttGenerateRandomID(_clientID, 23);

  // Attempt to connect to a Broker
  oled.clearMsgArea();
  oled.println(MQTT_HOST);
  oled.display();

  int error = feather.mqttConnect(MQTT_HOST, MQTT_PORT, (char *)_clientID, MQTT_AIOUSERNAME, MQTT_AIOKEY);
  if (error == 0)
  {
    oled.println("Connected!");
    oled.display();
    mqtt_connected = true;
  }
  else
  {
    oled.print("Failed! Error: ");
    oled.println(error, HEX);
    oled.display();
    delay(3000);
  }

  return error;
}

/**************************************************************************/
/*!
    @brief This function is called whenever a new event occurs
*/
/**************************************************************************/
void mqttCallback(mqtt_evt_opcode_t event, uint16_t len, uint8_t* data)
{
  oled.clearMsgArea();

  switch (event)
  {
    case MQTT_EVT_DISCONNECTED:
      oled.println("MQTT Disconnected");
      oled.display();
      mqtt_connected = false;
      break;
    case MQTT_EVT_TOPIC_CHANGED:
      oled.println("MQTT Update");
      // Print the message
      // [0:7]: UTC time & date (64-bit integer)
      // [8: ]: "topic=value"
      for (int i = 8; i < len; i++)
      {
        oled.write(data[i]);
      }
      oled.display();
      break;
    default:
      break;
  }
}
#endif

#if VBAT_ENABLED
/**************************************************************************/
/*!
    @brief
*/
/**************************************************************************/
void updateVbat() 
{
  int   vbatADC   = 0;         // The raw ADC value off the voltage div
  float vbatFloat = 0.0F;      // The ADC equivalent in millivolts
  float vbatLSB   = 0.80566F;  // mV per LSB

  // Read the analog in value:
  vbatADC = analogRead(VBAT_PIN);
  vbatADC = analogRead(VBAT_PIN);

  // Multiply the ADC by mV per LSB, and then
  // double the output to compensate for the
  // 10K+10K voltage divider
  vbatFloat = ((float)vbatADC * vbatLSB) * 2.0F;

  oled.setBattery(vbatFloat/1000);

  // Push VBAT out to MQTT if possible
  #if MQTT_ENABLED
  if (mqtt_connected)
  {
    char buffer[7];
    itoa((int)vbatFloat, buffer, 10);
    feather.mqttPublish(MQTT_TOPIC_VBAT, buffer, MQTT_QOS, MQTT_RETAIN);
  }
  #endif
}
#endif

/**************************************************************************/
/*!
    @brief
*/
/**************************************************************************/
void setup()
{
  // Wait for Serial Monitor
  // while(!Serial);
  
  // Setup the LED pin
  pinMode(BOARD_LED_PIN, OUTPUT);

  // Setup the OLED display
  oled.init();
  oled.clearDisplay();

  // Get a VBAT reading before refreshing if VBAT is available
  #if VBAT_ENABLED
  pinMode(VBAT_PIN, INPUT_ANALOG);
  oled.setBatteryIcon(true);
  updateVbat();
  #endif

  // Refresh the screen
  oled.refreshIcons();
  oled.clearMsgArea();

  // Try to connect to the AP
  if ( !connectAP() )
  {
    // Enter a while(1) loop here since any connection error
    // is handled in .connectAP() above
    while(1);
  }

  #if MQTT_ENABLED
  // Register the mqtt callback handler
  feather.addMqttCallBack(mqttCallback);

  // Connect to broker
  int mqtt_error = connectBroker();
  if (mqtt_error == 0)
  {
    mqtt_connected = true;
  }
  #endif

  #if SENSOR_TSL2561_ENABLED
  if (tsl.begin()) 
  {
    _tslFound = true;
    tsl.enableAutoRange(true);
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
    // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
    // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
  }
  #endif
}

/**************************************************************************/
/*!
    @brief
*/
/**************************************************************************/
void loop()
{
  // Update the battery level
  #if VBAT_ENABLED
  updateVbat();
  #endif
  
  if ( feather.connected() )
  {
    // Update the RSSI value
    int8_t rssi = feather.RSSI();
    oled.setRSSI(rssi);

    // Get a light sample and publish to MQTT if available
    #if SENSOR_TSL2561_ENABLED
      if (_tslFound)
      {
        oled.clearMsgArea();
        sensors_event_t event;
        // Get a new data sample
        bool sensor_data = tsl.getEvent(&event);
        if (sensor_data)
        {
          // Push data out to MQTT Broker if requested
          #if MQTT_ENABLED
            if (mqtt_connected)
            {
              char buffer[7];
              itoa(event.light, buffer, 10);
              if (feather.mqttPublish(MQTT_TOPIC_TSL2561_LUX, buffer, MQTT_QOS, MQTT_RETAIN) == 0)
              {
                oled.clearMsgArea();
                oled.print("Lux -> MQTT: ");
                oled.println(event.light);
                oled.display();
              }
              else
              {
               oled.clearMsgArea();
               oled.println("Publish failed");
               oled.display();
              }
            }
          #endif
        }
        else
        {
           oled.clearMsgArea();
           oled.println("Sensor failed");
           oled.display();
           Serial.println("Sensor failed");
        }
      }
    #endif
  }
  else
  {
    // The connection was lost ... reset the status icons
    oled.setConnected(false);
    oled.setRSSI(0);
    oled.setIPAddress(0);
    oled.clearMsgArea();
  }

  oled.refreshIcons();
  togglePin(BOARD_LED_PIN);
  delay(10000);
}

