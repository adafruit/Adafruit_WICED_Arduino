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
#include "adafruit_spiflash.h"

/* This example use SpiFlash print out the SPI Flash's content up to
 * MAX_LEVEL level of directories (including root)
 */

/*
 * WARNING: This example uses recursive call to print out directory tree
 * be extra careful, high number of MAX_LEVEL can cause memory overflow
 */
#define MAX_LEVEL   2

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(115200);

  // Wait for the USB serial to connect. Needed for native USB port only.
  while (!Serial) delay(1);
    
  Serial.println("SPI Flash List Files Example");
  
  SpiFlash.begin();

  // Print whole directory tree of root whose level is 0
  printTreeDir("/", 0);

  // Print prompt
  Serial.println();
  Serial.println("Enter anything to print directory tree (again):");
}

// the loop function runs over and over again forever
void loop() 
{
  // Waiting until there is any user input and echo
  char* input = getUserInput();
  Serial.println(input);

  // Print whole directory tree of root whose level is 0
  printTreeDir("/", 0);
}

/**************************************************************************/
/*!
    @brief  Print out whole directory tree of an folder
            until the level reach MAX_LEVEL
*/
/**************************************************************************/
void printTreeDir(const char* path, uint8_t level)
{
  // Open the input folder
  FatDir dir;
  
  dir.open(path);
  
  // Print root
  if (level == 0)
  {
    Serial.println("root");
  }else
  {
    // Change current directory to this folder to be able to open 
    // its sub folder with only folder name (relative path)
    SpiFlash.cd( path );   
  }
  
  // File Entry Information which hold file attribute and name
  FileInfo finfo;

  // Loop through the directory  
  while( dir.read(&finfo) )
  {
    // Indentation according to dir level
    for(int i=0; i<level; i++) 
    {
      Serial.print("|  ");
    }
    Serial.print("|_ ");
    Serial.print( finfo.name() );

    if ( finfo.isDirectory() ) 
    {
      Serial.println("/");

      // ATTENTION recursive call to print sub folder with level+1 !!!!!!!!
      // High number of MAX_LEVEL can cause memory overflow
      if ( level < MAX_LEVEL )
      {
        printTreeDir( finfo.name(), level+1 );
      }
    }else
    {
      // Print file size starting from position 50
      int pos = level*3 + 3 + strlen(finfo.name());

      // Print padding
      for (int i=pos; i<50; i++) Serial.print(' ');

      // Print at least one extra space in case current position > 50
      Serial.print(' ');

      // Print size in KB
      Serial.print( finfo.size() / 1024 );
      Serial.println( " KB");
    }
  }

  dir.close();

  // Change back to Parent if not root
  if (level != 0)
  {
    SpiFlash.cd( ".." );    
  }
}


/**************************************************************************/
/*!
    @brief  Get user input from Serial
*/
/**************************************************************************/
char* getUserInput(void)
{
  static char inputs[64+1];
  memset(inputs, 0, sizeof(inputs));

  // wait until data is available
  while( Serial.available() == 0 ) { delay(1); }

  uint8_t count=0;
  do
  {
    count += Serial.readBytes(inputs+count, 64);
  } while( (count < 64) && Serial.available() );

  return inputs;
}


