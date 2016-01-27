/*
  Connect to a WiFi access point. If success, perform an ICMP ping against 
  a specified IP address, a DNS lookup of the specified domain name and get
  the current UTC date and time using ISO 8601 standard. Then disconnect 
  from the given access point

  The ping response time, IPv4 address associated with the supplied domain
  name and string of current UTC date & time are displayed on Serial Monitor
  
  author: huynguyen 
 */

#include "adafruit_wifi.h"

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board    
*/
/**************************************************************************/
void setup()
{
  pinMode(BOARD_LED_PIN, OUTPUT);

  // initialize serial port for input and output
  Serial.begin(11500);

  // wait for Serial
  while (!Serial) delay(1);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() {
  // Connect to AP
  if ( feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.print("Connected to AP with SSID = ");
    Serial.print(WLAN_SSID);

    uint8_t ping_time[4];
    char* ip = "207.58.139.247";
    if (feather.ping(ip, ping_time) == ERROR_NONE)
    {
      uint32_t time;
      memcpy(&time, &ping_time[0], sizeof(time));
      Serial.print("Given IP address = "); Serial.println(ip);
      Serial.print("Ping time = "); Serial.print(time); Serial.println(" ms");
    }
    else
      Serial.println("Ping Error!");

    IPAddress ipv4;
    char* dns = "adafruit.com";
    if ( feather.hostByName(dns, ipv4) )
    {
      Serial.print("The IPv4 address of domain name \"");
      Serial.print(dns); Serial.print("\": ");
      Serial.println(ipv4);
    }
    else
    {
      Serial.print("DNS Lookup Error!: ");
      Serial.println(feather.errno(), HEX);
    }

    char iso8601_time[27];  // Length of UTC day and time in
                            // ISO 8601 standard is 28 bytes (including '\0')
    if (feather.getTime(iso8601_time) == ERROR_NONE)
    {
      Serial.print("Current UTC time and date (ISO 8061 standard): ");
      for (int i = 0; i < 27; i++)
        Serial.write(iso8601_time[i]);
    }
    else
      Serial.println("Get Time Error!");

    Serial.println("");
  }
  else
    Serial.println("Connect Error!");

  // Stop AP mode
  if (feather.disconnectAP() == ERROR_NONE)
  {
    Serial.println("Disconnected from AP");
  }
  else
    Serial.println("Disconnect Error!");

  Serial.println("");
  togglePin(BOARD_LED_PIN);
  delay(10000);   // Delay 10 seconds before next loop
}
