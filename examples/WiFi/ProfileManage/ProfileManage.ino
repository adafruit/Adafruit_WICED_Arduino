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

char ssid[]   = "yourSSID";     //  your network SSID (name)
char pass[]   = "yourPassword";  // your network password
int  enc_type =  ENC_TYPE_WPA2_AES;

void setup() {
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) delay(1); 
  
  // Print AP profile list
  Serial.println("Saved AP profile");
  Serial.println("ID Sec  SSID");
  for(uint8_t i=0; i<WIFI_MAX_PROFILE; i++)
  {
    char * profile_ssid = Feather.profileSSID(i);
    
    Serial.printf("%02d ", i);
    if ( profile_ssid != NULL )
    {
      printEncryptionType( Feather.profileEncryptionType(i) );
      Serial.print(" ");
      Serial.println(profile_ssid);
    }else
    {
      Serial.println("Not Available ");
    }
  }

  if ( Feather.checkProfile(ssid) )
  {
    Serial.print(ssid);
    Serial.println(" is already in profile list");
  }
  else
  {
    Serial.print("Adding ");
    Serial.print(ssid);
    Serial.println(" to profile list");
    
    Feather.addProfile(ssid, pass, enc_type);
  }
  
  // attempt to connect to Wifi network:
  do {
    Serial.println("Attempting to connect with saved profile");
  } while( !Feather.connect() );
  
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = Feather.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print your MAC address:
  byte mac[6];
  Feather.macAddress(mac);
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
  IPAddress subnet = Feather.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = Feather.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(Feather.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  Feather.BSSID(bssid);
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
  long rssi = Feather.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  Serial.print("Encryption Type: ");
  printEncryptionType( Feather.encryptionType() );
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

