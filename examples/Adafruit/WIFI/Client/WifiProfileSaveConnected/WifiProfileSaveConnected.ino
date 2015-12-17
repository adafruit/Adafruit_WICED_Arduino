/**************************************************************************/
/*!
    @file     WifiProfileSaveConnected.ino
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2015, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#include <WiFi101.h>

char ssid[]   = "yourNetwork";     //  your network SSID (name)
char pass[]   = "secretPassword";  // your network password

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    delay(1); // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Clear all AP profiles");
  WiFi.clearProfiles();
  
  // attempt to connect to Wifi network:
  do {
    Serial.print("Attempting to connect to ssid: ");
    Serial.println(ssid);
  } while( WiFi.begin(ssid, pass) != WL_CONNECTED);
  
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
  
  // Save the current AP Profile
  Serial.print("Saving current connected network to profile list ... ");
  if ( WiFi.saveProfile() )
  {
    Serial.println("Done");
  }else
  {
    Serial.println("Failed");
  }
  
  // Print AP profile list
  Serial.println("Saved AP profile");
  Serial.println("ID Sec  SSID");
  for(uint8_t i=0; i<WIFI_MAX_PROFILE; i++)
  {
    char * profile_ssid = WiFi.profileSSID(i);

    Serial.printf("%02d ", i);
    if ( profile_ssid != NULL )
    {
      printEncryptionType( WiFi.profileEncryptionType(i) );
      Serial.print(" ");
      Serial.println(profile_ssid);
    }else
    {
      Serial.println("Not Available ");
    }
  }
  
  Serial.println("Disconnecting from AP");
  WiFi.disconnect();
  
  Serial.println("Reconnecting with the saved profile");
  WiFi.begin();
}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  Serial.print("Encryption Type: ");
  printEncryptionType( WiFi.encryptionType() );
  Serial.println();
}

void printEncryptionType(int32_t thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
    case ENC_TYPE_WEP_SHARED:
      Serial.print("WEP");
      break;

    case ENC_TYPE_WPA_TKIP:
    case ENC_TYPE_WPA_AES:
      Serial.print("WPA");
      break;

    case ENC_TYPE_WPA2_AES:
    case ENC_TYPE_WPA2_TKIP:
    case ENC_TYPE_WPA2_MIXED:
      Serial.print("WPA2");
      break;

    case ENC_TYPE_OPEN:
      Serial.print("None");
      break;

//    case ENC_TYPE_AUTO:
//      Serial.print("Auto");
//      break;
  }
}

