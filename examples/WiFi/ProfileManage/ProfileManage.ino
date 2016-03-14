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

/* This example displays the current SSID profile(s) in NVM, saving
 * WLAN_SSID if it is not already in the list. It then attempts to
 * connect using the data stored in the profile list ('connect(void)').
 */

#include <adafruit_feather.h>

#define WLAN_SSID     "yourSSID"
#define WLAN_PASS     "yourPassword"
#define ENC_TYPE      ENC_TYPE_WPA2_AES

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println(F("Profile Manage Example\r\n"));

  // Print all software versions
  Feather.printVersions();

  // Print AP profile list
  Serial.println("Saved AP profile");
  Serial.println("ID SSID                 Sec");
  for(uint8_t i=0; i<WIFI_MAX_PROFILE; i++)
  {
    char * profile_ssid = Feather.profileSSID(i);
    int32_t profile_enc = Feather.profileEncryptionType(i);

    Serial.printf("%02d ", i);
    if ( profile_ssid != NULL )
    {
      Serial.printf("%-20s ", profile_ssid);
      Feather.printEncryption( profile_enc );
      Serial.println();
    }else
    {
      Serial.println("Not Available ");
    }
  }
  Serial.println();

  // Add defined SSID to profile if not already
  if ( Feather.checkProfile(WLAN_SSID) )
  {
    Serial.printf("\"%s\" : is already in profile list\r\n", WLAN_SSID);
  }
  else
  {
    Serial.print("Adding ");
    Serial.print(WLAN_SSID);
    Serial.println(" to profile list");

    Feather.addProfile(WLAN_SSID, WLAN_PASS, ENC_TYPE);
  }

  // Connect to Wifi network using saved profile list
  do {
    Serial.println("Attempting to connect with saved profile");
  } while( !Feather.connect() );

  // Connected: Print network info
  Feather.printNetwork();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  // Print every 10 seconds
  delay(10000);

  Serial.print("Signal strength (RSSI): ");
  Serial.println( Feather.RSSI() );
}
