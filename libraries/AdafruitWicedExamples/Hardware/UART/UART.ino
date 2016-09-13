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

/* This example demonstrate that USBSerial and all HW UARTs (1,2,4) are working.
 * Sketch will echo any characters from any Serials to all the Serials.
 */

#include <Arduino.h>

const int baudrate = 115200;

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin (baudrate);  // USB monitor
  Serial1.begin(baudrate);  // HW UART1
  Serial2.begin(baudrate);  // HW UART2
  
  // wait for the serial port to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println ("UART demo: Serial Monitor");
  Serial.printf  ("Badurate : %d\r\n", baudrate);
  
  Serial1.println("UART demo: HWUART1");
  Serial1.printf ("Badurate : %d\r\n", baudrate);
  
  Serial2.println("UART demo: HWUART2");
  Serial2.printf ("Badurate : %d\r\n", baudrate);
}

void printAll(char ch)
{ 
  Serial.print (ch);
  Serial1.print(ch);
  Serial2.print(ch);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  char ch;
  
  // From Serial monitor to All
  if ( Serial.available() )
  {
    ch = (char) Serial.read();
    printAll(ch); 
  }

  // From HW UART1 to All
  if ( Serial1.available() )
  {
    ch = (char) Serial1.read();
    printAll(ch); 
  }
  
  // From HW UART2 to All
  if ( Serial2.available() )
  {
    ch = (char) Serial2.read();
    printAll(ch); 
  }
}
