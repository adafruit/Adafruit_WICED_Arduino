/*
  Connect to a WiFi access point. If success, perform an ICMP ping against 
  a specified IP address and a DNS lookup of the specified domain name. Then
  disconnect from the given access point

  The ping response time and IPv4 address associated with the supplied domain
  name are displayed on Serial Monitor
  
  author: huynguyen 
 */

#include "adafruit_wifi.h"

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board    
*/
/**************************************************************************/
void setup()
{
  pinMode(BOARD_LED_PIN, OUTPUT);
  delay(5000);
  
  // initialize serial port for input and output
//  Serial.begin(11500);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() {
  // put your main code here, to run repeatedly:
  
  // Connect to an AP
  char* ssid = "ADAFRUIT";
  char* pass = "12345678";
  if (wiced.connectAP(ssid, pass) == ERROR_NONE)
  {
    Serial.print("Connected to AP with SSID = ");
    Serial.print(ssid);
    Serial.print(" and PASSWORD = ");
    Serial.println(pass);
  }
  else
    Serial.println("Connect Error!");

  uint8_t ping_time[4];
  char* ip = "207.58.139.247";
  if (wiced.ping(ip, ping_time) == ERROR_NONE)
  {
    uint32_t time;
    memcpy(&time, &ping_time[0], sizeof(time));
    Serial.print("Given IP address = "); Serial.println(ip);
    Serial.print("Ping time = "); Serial.print(time); Serial.println(" ms");
  }
  else
    Serial.println("Ping Error!");

  uint8_t ipv4[4];
  char* dns = "adafruit.com";
  if (wiced.dnsLookup(dns, ipv4) == ERROR_NONE)
  {
    Serial.print("The IPv4 address of domain name \"");
    Serial.print(dns); Serial.print("\": ");
    Serial.print(ipv4[3]); Serial.print(".");
    Serial.print(ipv4[2]); Serial.print(".");
    Serial.print(ipv4[1]); Serial.print(".");
    Serial.println(ipv4[0]);
  }
  else
    Serial.println("DNS Lookup Error!");

  // Stop AP mode
  if (wiced.disconnectAP() == ERROR_NONE)
  {
    Serial.println("Disconnected from AP");
  }
  else
    Serial.println("Disconnect Error!");

  Serial.println("");
  togglePin(BOARD_LED_PIN);
  delay(10000);   // Delay 10 seconds before next loop
}
