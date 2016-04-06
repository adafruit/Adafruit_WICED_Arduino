#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <Adafruit_FeatherOLED_WiFi.h>
#include <Adafruit_Sensor.h>
#include <adafruit_feather.h>
#include <adafruit_mqtt.h>
#include <adafruit_aio.h>

#define WLAN_SSID                 "SSID"
#define WLAN_PASS                 "PASSWORD"

#define VBAT_ENABLED              1
#define VBAT_PIN                  PA1

#define SENSOR_TSL2561_ENABLED    0
#if SENSOR_TSL2561_ENABLED
  #include <Adafruit_TSL2561_U.h>
  bool _tslFound = false;
  Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
#endif

#define AIO_ENABLED               0
#define AIO_USERNAME              "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_KEY                   "...your AIO key..."

#define FEED_VBAT                 "vbat"
#define FEED_TSL2561_LUX          "lux"

AdafruitAIO                        aio(AIO_USERNAME, AIO_KEY);
AdafruitAIOFeedGauge<float>        feedVBAT(&aio, FEED_VBAT);
AdafruitAIOFeedGauge<float>        feedLUX (&aio, FEED_TSL2561_LUX);

Adafruit_FeatherOLED_WiFi  oled = Adafruit_FeatherOLED_WiFi();

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
  if ( Feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    int8_t rssi = Feather.RSSI();
    uint32_t ipAddress = Feather.localIP();
    oled.setConnected(true);
    oled.setRSSI(rssi);
    oled.setIPAddress(ipAddress);
    oled.refreshIcons();
    oled.clearMsgArea();
  }
  else
  {
    // Display the error message
    err_t err = Feather.errno();
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
        oled.print(Feather.errno());
        oled.print(":");
        oled.println(Feather.errstr());
        oled.refreshIcons(); // Refresh icons in case the text ran over
        break;
    }    
    oled.display();
    // Return false to indicate that we received an error (available in feather.errno)
    return false;
  }

  return true;
}

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
  if (AIO_ENABLED && aio.connected())
  {
    feedVBAT = vbatFloat/1000;
  }
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  // Wait for Serial Monitor
//  while(!Serial) delay(1);
   
  // Setup the LED pin
  pinMode(BOARD_LED_PIN, OUTPUT);

  // Setup the OLED display
  oled.init();
  oled.clearDisplay();

  // Initialize tsl sensor if enabled
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

  #if AIO_ENABLED
  // Attempt to connect to a Broker
  oled.clearMsgArea();
  oled.println("io.adafruit.com");
  oled.display();

  // Connect to AIO server
  if ( aio.connect() )
  {
    oled.println("Connected!");
    oled.display();
  }else
  {
    oled.print("Failed! Error: ");
    oled.println(aio.errno(), HEX);
    oled.display();
    delay(3000);
  }
  
  // Follow feed if enabled
  if ( VBAT_ENABLED )
    feedVBAT.follow(aio_vbat_callback);
  
  // Follow feed if enabled
  if ( SENSOR_TSL2561_ENABLED )
    feedLUX.follow(aio_vbat_callback);
    
  #endif
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  // Update the battery level
  if ( VBAT_ENABLED )
    updateVbat();
    
  if ( Feather.connected() )
  {
    // Update the RSSI value
    int8_t rssi = Feather.RSSI();
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
        if (AIO_ENABLED && aio.connected())
        {
          feedLUX = event.light;

          oled.clearMsgArea();
          oled.print("Lux -> AIO: ");
          oled.println(event.light);
          oled.display();
        }
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

/**************************************************************************/
/*!
    @brief AIO callback when there is new value with Feed VBAT
*/
/**************************************************************************/
void aio_vbat_callback(float value)
{
//  oled.println("AIO VBAT: ");
//  oled.display();
}

/**************************************************************************/
/*!
    @brief AIO callback when there is new value with Feed LUX
*/
/**************************************************************************/
void aio_lux_callback(float value)
{
//  oled.println("AIO LUX:");
//  oled.display();
}

