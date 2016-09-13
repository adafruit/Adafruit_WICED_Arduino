/*********************************************************************
 This is an example for our WICED Feather WIFI modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This example uses the AdafruitHTTPServer class to create a file browser for
 * the SPI flash file system. The sketch assumes the SPI flash is enabled
 * by soldering the optional solder jumper for SPI flash 'closed'.
 *
 * HTTP resources used in this example are included in resources folder.
 *
 * To convert static web files (html, jpg etc..) into C headers, use the
 * included tools/pyresource utility. 
 * 
 * For example: To convert all the files in './resources' run:
 *
 *    $ python ../../../tools/pyresource/pyresource.py resources
 *
 * pyresource will convert each file to a header file where '.' & '-' are
 * replaced by '_'. Each folder separator ('/') is repalce by '_dir_':
 *
 *    images/logo.png --> images_dir_logo_png.h
 *
 * The corresponding HTTPResource variable is 'images_dir_logo_png'
 *
 * For your convenience, a master header file named resources.h is also
 * generated and includes all of the generated header files. You only
 * need to include this one file via:
 *
 *      #include "resources.h
 */

#include <adafruit_feather.h>
#include <adafruit_http_server.h>
#include "adafruit_spiflash.h"
#include "resources.h"

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

#define PORT                 80            // The TCP port to use
#define MAX_CLIENTS          3

int ledPin = PA15;

void filesystem_generator (const char* url, const char* query, void* http_request);
void file_not_found_generator (const char* url, const char* query, void* http_request);

HTTPPage pages[] = 
{
  HTTPPageRedirect("/", "/index.html"), // redirect root to index.html
  HTTPPage("/404.html"          , HTTP_MIME_TEXT_HTML       , file_not_found_generator ),

  // Static Content
  HTTPPage("/32px.png"          , HTTP_MIME_IMAGE_PNG       , &_32px_png         ),
  HTTPPage("/adafruit_logo.png" , HTTP_MIME_IMAGE_PNG       , &adafruit_logo_png ),
  HTTPPage("/favicon.ico"       , HTTP_MIME_IMAGE_MICROSOFT , &favicon_ico       ),
  HTTPPage("/file_sprite.png"   , HTTP_MIME_IMAGE_PNG       , &file_sprite_png   ),
  HTTPPage("/index.html"        , HTTP_MIME_TEXT_HTML       , &index_html        ),

  // Dynamic Content
  HTTPPage("/filesystem.json"    , HTTP_MIME_JSON           , filesystem_generator),
};

uint8_t pagecount = sizeof(pages)/sizeof(HTTPPage);

// Declare HTTPServer with max number of pages
AdafruitHTTPServer httpserver(pagecount);

/******************************************************************************/
/**
 * Filesytem contents such as directory listings or file contents.
 *
 * Ex use: http://192.168.0.124/filesystem.json?path=readme.txt
 *
 * Link is seperated to url and query
 *
 * @param url           url of this page, should be "/filesystem.json"
 * @param query         query string after '?' e.g "path=/readme.txt"
 *
 * @param http_request
 */
/******************************************************************************/
void filesystem_generator (const char* url, const char* query, void* http_request)
{
  (void) url;
  (void) http_request;

  // JSON start bracket
  httpserver.print("[");

  // query must start with path=, e.g "path=myfolder/readme.txt"
  const char* prefix = "path=";
  if ( 0 == strncmp(query, prefix, strlen(prefix)) )
  {   
    const char* filepath = query + strlen(prefix);
    
    // No path means root level access
    if(*filepath == 0) filepath = "/";

    // Check if the file exists
    if ( SpiFlash.exists(filepath) )
    { 
      // If it's a directory, list the entry
      if ( SpiFlash.isDirectory(filepath) )
      {
        FatDir dir;
        dir.open(filepath);

        bool is_first = true; // Print a seperator

        // Read directory items in sequence (re-use finfo variable)
        FileInfo finfo;
        while( dir.read(&finfo) )
        {
          // Print seperator
          if (!is_first) httpserver.print(",\r\n");
          is_first = false;

          // Send response in JSON format e.g {"name" : filename, "data_type": "folder" }
          httpserver.print("{\"name\": \"");
          httpserver.print(finfo.name());
          httpserver.print("\", \"data_type\": \"");
          httpserver.print( finfo.isDirectory() ? "folder" : "file" );
          httpserver.print("\"}");
        }

        dir.close();
      }
      // if this is a file, read the content
      else
      {
        FatFile file;
        if ( file.open(filepath, FAT_FILE_READ) )
        {
          while( file.available() )
          {
            uint8_t buffer[64];
            uint32_t count = file.read(buffer, 64);

            // If not printable --> binary file, skip
            if ( !isPrintable(buffer, count) )
            {
              httpserver.print("Binary file is currently not supported!");
              break;
            }
            httpserver.write(buffer, count);
          }
        }
        file.close();
      }
    }
  }

  // JSON end bracket
  httpserver.print("]");
}

/**************************************************************************/
/*!
    @brief  This setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the USB serial to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("HTTP Server SPI Flash File Browser Example\r\n");

  // Initalize SPI Flash and FAT filesystem
  SpiFlash.begin();
  
  // Print all software versions
  Feather.printVersions();

  // Try to connect to an AP
  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the HTTP client to auto print error codes and halt on errors
  httpserver.err_actions(true, true);

  // Configure HTTP Server Pages
  Serial.println("Adding Pages to HTTP Server");
  httpserver.addPages(pages, pagecount);

  Serial.print("Starting HTTP Server ... ");
  httpserver.begin(PORT, MAX_CLIENTS);
  Serial.println(" running");
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  togglePin(ledPin);
  delay(1000);
}

/**************************************************************************/
/*!
    @brief  HTTP Page 404 generator
*/
/**************************************************************************/
void file_not_found_generator (const char* url, const char* query, void* http_request)
{
  (void) url;
  (void) query;
  (void) http_request;

  httpserver.print("<html><body>");
  httpserver.print("<h1>Error 404 File Not Found!</h1>");
  httpserver.print("<br>");

  httpserver.print("Available pages are:");
  httpserver.print("<br>");

  httpserver.print("<ul>");
  for(int i=0; i<pagecount; i++)
  {
    httpserver.print("<li>");
    httpserver.print(pages[i].url);
    httpserver.print("</li>");
  }
  httpserver.print("</ul>");

  httpserver.print("</body></html>");
}

/**************************************************************************/
/*!
    @brief  Check if a string is printable
*/
/**************************************************************************/
bool isPrintable(uint8_t* buf, uint32_t count)
{
  for(uint32_t i=0; i<count; i++)
  {
    int ch = (int) buf[i];
    if ( !isprint(ch) && !isspace(ch) ) return false;
  }

  return true;
}

/**************************************************************************/
/*!
    @brief  Connect to the defined access point (AP)
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

/**************************************************************************/
/*!
    @brief  TCP/HTTP disconnect callback
*/
/**************************************************************************/
void disconnect_callback(void)
{
  Serial.println();
  Serial.println("---------------------");
  Serial.println("DISCONNECTED CALLBACK");
  Serial.println("---------------------");
  Serial.println();

  httpserver.stop();
}
