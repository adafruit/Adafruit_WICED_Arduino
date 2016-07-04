
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

#include "adafruit_feather.h"
#include "adafruit_email.h"

/* This example demonstrates how to use AdafrtuiEmail class
 * to send out an email
 *
 * To run this demo:
 * 1. Change WLAN_SSID & WLAN_PASS to your network
 * 2. Change your EMAIL_ADDRESS & EMAIL_USERNAME & EMAIL_PASSWORD
 * according to your email account. Note you will need to generate
 * an application password if you have enabled 2-step verification.
 * 3. Configure your SMTP Server
 * 4. Change Recipients (To, CC, BCC). Notes each list is comma separated.
 *
 * NOTE: Only tested with gmail server.
 */

// Network
#define WLAN_SSID              "yourSSID"
#define WLAN_PASS              "yourPassword"

// Email Account
#define EMAIL_ADDRESS          "yourEmail"
#define EMAIL_USERNAME         "yourEmailUsername"
#define EMAIL_PASSWORD         "yourEmailPassword"

// SMTP Server
#define OUTGOING_SMTP_SERVER   "smtp.gmail.com"
#define OUTGOING_SMTP_PORT     587
#define OUTGOING_TLS           1

// Recipients
#define RECIPIENTS_TO_ADDRS    EMAIL_ADDRESS // self send
#define RECIPIENTS_CC_ADDRS    NULL
#define RECIPIENTS_BCC_ADDRS   NULL

// Mail
#define MAIL_SUBJECT           "Adafruit Wiced Feather"
#define MAIL_CONTENT           "Hello,\r\nMy name is WICED"
#define MAIL_SIGNATURE         "\r\nSent from WICED Feather"


AdafruitEmail Email;

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

  Serial.println("Send Mail Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  Email.begin(EMAIL_ADDRESS, EMAIL_USERNAME, EMAIL_PASSWORD);
  Email.err_actions(true, true);

  Serial.println("Configure SMTP Server");
  Email.outgoingServer(OUTGOING_SMTP_SERVER, OUTGOING_SMTP_PORT, OUTGOING_TLS);

  Serial.println("Set Recipients");
  Email.recipients(RECIPIENTS_TO_ADDRS, RECIPIENTS_CC_ADDRS, RECIPIENTS_BCC_ADDRS);

  Serial.println("Set signature (optional)");
  Email.signature(MAIL_SIGNATURE);

  Serial.print("Sending email ...");
  Email.send(MAIL_SUBJECT, MAIL_CONTENT);
  Serial.println("Done");

  Email.stop();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  togglePin(PA15);
  delay(1000);
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
