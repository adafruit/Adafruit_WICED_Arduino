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

/* This example attempts to:
 * - Connect to your SSID
 * - Save the SSID info to NVM profile list if connected successfully
 * - Disconnects then re-connects using the saved profile
 */

#include <adafruit_feather.h>

#define WLAN_SSID     "yourSSID"
#define WLAN_PASS     "yourPassword"

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

  Serial.println(F("Profile Save Connected Example\r\n"));

  Serial.print("\r\nClear all AP profiles ... ");
  Feather.clearProfiles();
  Serial.println("OK");

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Save the current AP Profile
  Serial.print("Saving current connected network to profile list ... ");
  if ( Feather.saveConnectedProfile() )
  {
    Serial.println("OK");
  }else
  {
    Serial.println("Failed");
  }

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

  // Disconnect to test connecting with saved profile
  Serial.println("Disconnecting from AP ... ");
  Feather.disconnect();
  Serial.println("OK");

  Serial.println("Reconnecting with the saved profile (should be quicker)");
  Feather.connect(); // no parameters --> using saved profiles
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

  Feather.printNetwork();
}


/**************************************************************************/
/*!
    @brief  Connect to defined Access Point
*/
/**************************************************************************/
bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Please wait while connecting to: '" WLAN_SSID "' ... ");

  if ( Feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errno());
    Serial.println();
  }
  Serial.println();

  return Feather.connected();
}
