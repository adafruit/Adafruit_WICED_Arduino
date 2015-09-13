/*
  Basic WiFi Operations: Scan AP, start AP, connect to AP
  
  The results are showed up using Serial Monitor
  
  author: huynguyen 
 */

#include "adafruit_wifi.h"
#include "wifi_constants.h"

uint8_t ap_details[5 * AP_INFO_PACKET_SIZE + 2];

/**************************************************************************/
/*!
    @brief  Display AP information after scanning (via the Serial Monitor)
*/
/**************************************************************************/
void print_ap_details(uint8_t* ap_details)
{
  uint16_t buffer_size;
  memcpy(&buffer_size, ap_details, sizeof(buffer_size));

  int i = sizeof(buffer_size);
  while (i < buffer_size)
  {
    // SSID field
    uint8_t ssid_len = ap_details[i];
    for (int j = i; j < i+ssid_len; j++)
    {
      Serial.write(ap_details[j]);
    }
    i += ssid_len;
    Serial.print("\t");

    // MAC address
    for (int j = i; j < i + 6; j++)
    {
      if (ap_details[j] < 0x10) { Serial.print("0"); }
      Serial.print(ap_details[j], HEX);
      if (j < i + 5) { Serial.print(":"); }
    }
    i += 6;
    Serial.print("\t");

    // Receive Signal Strength in dBm
    uint16_t rss;
    memcpy(&rss, &ap_details[i], sizeof(rss));
    i += sizeof(rss);
    Serial.print(rss);
    Serial.print("\t");
    
    // Maximum data rate in kilobits/s
    uint32_t max_data_rate;
    memcpy(&max_data_rate, &ap_details[i], sizeof(max_data_rate));
    i += sizeof(max_data_rate);
    Serial.print(max_data_rate);
    Serial.print("\t");

    // Network type
    if (ap_details[i++] == WICED_BSS_TYPE_ADHOC)
    {
      Serial.print("Adhoc");
    }
    else
      Serial.print("Infra");
    Serial.print("\t");
    
    // Security type
    uint32_t security_type;
    memcpy(&security_type, &ap_details[i], sizeof(security_type));
    i += sizeof(security_type);
    Serial.print(rss);
    
    if (security_type == WICED_SECURITY_OPEN) Serial.print("Open");
    else if (security_type == WICED_SECURITY_WEP_PSK) Serial.print("WEP");
    else if (security_type == WICED_SECURITY_WPA_TKIP_PSK) Serial.print("WPA TKIP");
    else if (security_type == WICED_SECURITY_WPA_AES_PSK) Serial.print("WPA AES");
    else if (security_type == WICED_SECURITY_WPA2_AES_PSK) Serial.print("WPA2 AES");
    else if (security_type == WICED_SECURITY_WPA2_TKIP_PSK) Serial.print("WPA2 TKIP");
    else if (security_type == WICED_SECURITY_WPA2_MIXED_PSK) Serial.print("WPA2 Mixed");
    else if (security_type == WICED_SECURITY_WPS_OPEN) Serial.print("WPS Open Mixed");
    else if (security_type == WICED_SECURITY_WPS_SECURE) Serial.print("WPS AES");
    else Serial.print("Unknown");
    Serial.print("\t");
    
    // Radio channel
    Serial.print(ap_details[i++]);
    Serial.print("\t");
    
    // Radio band
    Serial.print(ap_details[i++]);

    // New line for next accesspoint
    Serial.println("");
  }
}

// the setup function runs once when you press reset or power the board
void setup()
{
  pinMode(BOARD_LED_PIN, OUTPUT);
  
  // initialize serial port for input and output
  Serial.begin(11500);
  Serial.println("Basic WiFi Operations Example");
}

// the loop function runs over and over again forever
void loop()
{
  
  // Perform AP Scan
  if (wiced.scan(ap_details) == 0)       
    print_ap_details(ap_details);   // Display AP information via Serial Monitor
  else
    Serial.println("Scan error");
    
  togglePin(BOARD_LED_PIN);
  delay(5000);                      // Delay 5 seconds before next scan
}
