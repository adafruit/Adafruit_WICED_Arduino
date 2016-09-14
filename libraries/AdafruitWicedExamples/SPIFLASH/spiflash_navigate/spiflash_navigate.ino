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

/* This example use SpiFlash to navigate through the filesystem.
 * User enters directory name to navigate to it ( ".." for parent folder).
 * If file is enter, this sketch will dump its contents to serial.
 */

char abs_cwd[256];

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(115200);

  // Wait for the USB serial to connect. Needed for native USB port only.
  while (!Serial) delay(1);
    
  Serial.println("SPI Flash Dump File Example");
  
  SpiFlash.begin();

  printDir("/");

  Serial.println();
  Serial.print("Enter Filename to dump or Folder to navigate to: ");
}

// the loop function runs over and over again forever
void loop() 
{
  // Waiting until there is any user input and echo
  char* input = getUserInput();
  Serial.println(input);

  // Get File Info and check if it is File or Directory
  FileInfo finfo;

  if ( finfo.fetch(input) )
  {
    // Navigate to folder and display its contents
    if ( finfo.isDirectory() )
    {
      // print Directory contents
      printDir(input);

      // Change current directory to this folder to be able to open 
      // its sub folder with only folder name (relative path)
      SpiFlash.cd(input);
    }
    // Dump file contents
    else
    {
      FatFile file;
      
      if ( file.open(input, FAT_FILE_READ) )
      {
        while( file.available() )
        {
          Serial.write( (char) file.read() );
        }
      }

      file.close();
      Serial.println();      
    }
  }else
  {
    Serial.println("File not found");
  }

  Serial.println();
  Serial.print("Enter Filename to dump or Folder to navigate to: ");
}

/**************************************************************************/
/*!
    @brief  Print directory contents
*/
/**************************************************************************/
void printDir(const char* path)
{
  // Open the input folder
  FatDir dir;  
  dir.open(path);

  // Get Parent absolute path
  SpiFlash.cwd(abs_cwd, sizeof(abs_cwd));

  Serial.println();
  Serial.print(abs_cwd);

  // If Parent is root (first level directory), PWD already ending with '/'
  // Otherwise add '/' as folder seperator
  if ( strcmp(abs_cwd, "/") ) Serial.print('/');
  
  if ( strcmp(path, "/") == 0 )
  {
    Serial.print(" (root)");
  }else
  {
    Serial.print(path);
  }
  Serial.println();
  
  // File Entry Information which hold file attribute and name
  FileInfo finfo;

  // Loop through the directory  
  while( dir.read(&finfo) )
  {
    Serial.print("|_ ");
    Serial.print( finfo.name() );

    if ( finfo.isDirectory() ) 
    {
      Serial.println("/");
    }else
    {      
      // Print padding, file size starting from position 30
      for (int i=strlen(finfo.name()); i<30; i++) Serial.print(' ');

      // Print at least one extra space in case current position > 30
      Serial.print(' ');

      // Print size in KB
      uint32_t kb = finfo.size() < 1024;
      if (kb == 0) kb = 1; // less than 1KB still counts as 1KB
      
      Serial.print( kb );
      Serial.println( " KB");
    }
  }

  dir.close();
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

