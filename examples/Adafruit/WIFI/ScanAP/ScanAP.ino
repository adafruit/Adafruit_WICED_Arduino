/*
  Scanning for WiFi APs in range every 10 seconds
  
  The results are showed up using Serial Monitor
  
  author: huynguyen 
 */

#include "adafruit_wifi.h"
#include "wifi_constants.h"

/**************************************************************************/
/*!
    @brief  Display AP information after scanning (via the Serial Monitor)
*/
/**************************************************************************/
void print_ap_details(uint16_t buffer_size, uint8_t* ap_details)
{
  int i = 0;
  while (i < buffer_size)
  {
    // SSID field
    uint8_t ssid_len = ap_details[i++];
    for (int j = 0; j < 32; j++)
    {
      if (j < ssid_len)
        Serial.write(ap_details[j + i]);
      else
        Serial.print(F(" "));
    }
    i += ssid_len;
    Serial.print(F("\t"));

    // MAC address
    for (int j = i; j < i + 6; j++)
    {
      if (ap_details[j] < 0x10) { Serial.print(F("0")); }
      Serial.print(ap_details[j], HEX);
      if (j < i + 5) { Serial.print(":"); }
    }
    i += 6;
    Serial.print(F("\t"));

    // Receive Signal Strength in dBm
    uint16_t rss;
    memcpy(&rss, &ap_details[i], sizeof(rss));
    i += sizeof(rss);
    Serial.print((int16_t)rss);
    Serial.print(F("\t"));
    
    // Maximum data rate in kilobits/s
    uint32_t max_data_rate;
    memcpy(&max_data_rate, &ap_details[i], sizeof(max_data_rate));
    i += sizeof(max_data_rate);
    Serial.print(max_data_rate/1000.0, 2);
    Serial.print(F("\t"));

    // Network type
    if (ap_details[i++] == WICED_BSS_TYPE_ADHOC)
    {
      Serial.print(F("Adhoc"));
    }
    else
      Serial.print(F("Infra"));
    Serial.print(F("\t"));
    
    // Security type
    uint32_t security_type;
    memcpy(&security_type, &ap_details[i], sizeof(security_type));
    i += sizeof(security_type);
    
    if (security_type == WICED_SECURITY_OPEN)
      Serial.print(F("Open          "));
    else if (security_type == WICED_SECURITY_WEP_PSK)
      Serial.print(F("WEP           "));
    else if (security_type == WICED_SECURITY_WPA_TKIP_PSK)
      Serial.print(F("WPA TKIP      "));
    else if (security_type == WICED_SECURITY_WPA_AES_PSK)
      Serial.print(F("WPA AES       "));
    else if (security_type == WICED_SECURITY_WPA2_AES_PSK)
      Serial.print(F("WPA2 AES      "));
    else if (security_type == WICED_SECURITY_WPA2_TKIP_PSK)
      Serial.print(F("WPA2 TKIP     "));
    else if (security_type == WICED_SECURITY_WPA2_MIXED_PSK)
      Serial.print(F("WPA2 Mixed    "));
    else if (security_type == WICED_SECURITY_WPS_OPEN)
      Serial.print(F("WPS Open Mixed"));
    else if (security_type == WICED_SECURITY_WPS_SECURE)
      Serial.print(F("WPS AES       "));
    else 
      Serial.print(F("Unknown       "));
    Serial.print(F("\t"));
    
    // Radio channel
    Serial.print(ap_details[i++]);
    Serial.print(F("\t"));
    
    // Radio band
    Serial.print(ap_details[i++]);

    // New line for next accesspoint
    Serial.println(F(""));
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

  Serial.println(F("Scan Access Point Example\r\n"));
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever    
*/
/**************************************************************************/
void loop()
{
  // Perform AP Scan
  uint16_t buffer_size;
  uint8_t  ap_details[10 * AP_INFO_PACKET_SIZE];
  uint16_t error = wiced.scan(&buffer_size, ap_details);
  if (error == ERROR_NONE)
  {
    print_ap_details(buffer_size, ap_details);   // Display AP information via Serial Monitor
  }
  else
  {
    Serial.println(F("Scan Failed! Error: "));
    Serial.print(error, HEX);
  }

  Serial.println(F("\r\n"));
   
  togglePin(BOARD_LED_PIN);  // Toggle LED for debug
  delay(10000);              // Delay 10 seconds before next loop
}
