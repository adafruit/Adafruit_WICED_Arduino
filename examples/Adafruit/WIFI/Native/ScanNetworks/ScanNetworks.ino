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

#include <adafruit_wifi.h>

#define MAX_SCAN_NUM   20
wl_ap_info_t scan_result[MAX_SCAN_NUM];

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    delay(1); // wait for serial port to connect. Needed for native USB port only
  }

  // Print WiFi MAC address:
  printMacAddress();
}

void loop() {
  Serial.println();
  Serial.println("Scanning available networks...");
  listNetworks();

  Serial.println();
  Serial.println("Try again in 10 seconds");
  delay(10000);
}

void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];

  // print your MAC address:
  feather.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);  Serial.print(":");
  Serial.print(mac[4], HEX);  Serial.print(":");
  Serial.print(mac[3], HEX);  Serial.print(":");
  Serial.print(mac[2], HEX);  Serial.print(":");
  Serial.print(mac[1], HEX);  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scanned Networks **");
  int numSsid = feather.scanNetworks(scan_result, MAX_SCAN_NUM);
  if (numSsid < 0)
  {
    Serial.println("Couldn't get a wifi connection");
    return;
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  Serial.printf("ID SSID                 Enc  Ch Strength\n");
  for (int i = 0; i < numSsid; i++) {
    Serial.printf("%02d %-20s %s %02d %02d dBm", 
                  i, scan_result[i].ssid, 
                  getEncryptionType( scan_result[i].security ),
                  scan_result[i].channel,
                  scan_result[i].rssi);
    Serial.println();
  }
}

char* getEncryptionType(int32_t thisType) 
{
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
    case ENC_TYPE_WEP_SHARED:
      return "WEP";
    break;
      
    case ENC_TYPE_WPA_TKIP:
    case ENC_TYPE_WPA_AES:
    case ENC_TYPE_WPA_MIXED:
      return "WPA";
    break;
    
    case ENC_TYPE_WPA2_AES:
    case ENC_TYPE_WPA2_TKIP:
    case ENC_TYPE_WPA2_MIXED:
      return "WPA2";
    break;
      
    case ENC_TYPE_OPEN:
      return "Open";
    break;
      
//    case ENC_TYPE_AUTO:
//      Serial.println("Auto");
//      break;
  }
}



