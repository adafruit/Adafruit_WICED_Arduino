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

/* This example start SoftAP, register callback for client
 * join and leave event. The sketch will display the connected list
 * with MAC and RSSI.
 */

#include <adafruit_feather.h>
#include <adafruit_featherap.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"
#define WLAN_ENCRYPTION       ENC_TYPE_WPA2_AES
#define WLAN_CHANNEL          1

IPAddress apIP     (192, 168, 2, 1);
IPAddress apGateway(192, 168, 2, 1);
IPAddress apNetmask(255, 255, 255, 0);

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the Serial Monitor to open
  while (!Serial)
  {
    /* Delay required to avoid RTOS task switching problems */
    delay(1);
  }

  Serial.println("SoftAP Client RSSI Example\r\n");

  Serial.println("Configuring SoftAP\r\n");
  FeatherAP.err_actions(true, true);
  FeatherAP.setJoinCallback(client_join_callback);
  FeatherAP.setLeaveCallback(client_leave_callback);
  
  FeatherAP.begin(apIP, apGateway, apNetmask, WLAN_CHANNEL);

  Serial.println("Starting SoftAP\r\n");
  FeatherAP.start(WLAN_SSID, WLAN_PASS, WLAN_ENCRYPTION);
  FeatherAP.printNetwork();
}

void client_join_callback(const uint8_t mac[6])
{
  Serial.print("Client Joined: Mac = ");
  printMAC(mac);
  Serial.println();
}

void client_leave_callback(const uint8_t mac[6])
{
  Serial.print("Client Left  : Mac = ");
  printMAC(mac);
  Serial.println();
}

void printMAC(const uint8_t mac[6])
{
  for(int i=0; i<6; i++)
  {
    if (i > 0) Serial.print(':');
    Serial.printf("%02X", mac[i]);  
  }
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  Serial.println("ID MAC               RSSI");
  for(int i=0; i<FeatherAP.clientNum(); i++)
  {
    Serial.print(i);
    Serial.print("  ");
    
    printMAC( FeatherAP.clientMAC(i) );
    Serial.print(' ');

    Serial.println(FeatherAP.clientRSSI(i));  
  }
  Serial.println();


  Serial.println();
  Serial.println("Waiting 10 seconds before trying again");
  delay(10000);
}
