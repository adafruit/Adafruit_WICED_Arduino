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

/* This example use the AdafruitHTTP class to create a file browser for
 * SPI Flash file system
 */
/* This example use AdafruitHTTP to to create a file browser for
 * SPI Flash file system. Web's design are included in resources folder.
 *
 * - index.html: mainpage import data from thread.csv & heap.csv and use D3
 *               to draw charts.
 * - 32px.png:
 * - adafruit_logo.png :
 * - file_sprite.png:
 * - favicon.ico: favicon icon of Adafruit
 *
 * To convert web design files (html, jpg etc..) into C header, please use
 * included tools/pyresource. E.g command to convert all the files in resources
 * folder for this sketch is
 *    $ python ../../../tools/pyresource/pyresource.py resources
 *
 * pyresource will convert each file to a header file with '.' & '-' is
 * replaced by '_' . Each folder separation '/' is repalce by '_dir_'.
 *    images/logo.png --> images_dir_logo_png.h
 * The HTTPResource variable is images_dir_logo_png
 *
 * For your convenience, a master header named resources.h is also generated
 * and include all of generated header file. Your sketch only need to
 *      #include "resources.h
 */

#include <adafruit_feather.h>
#include <adafruit_http_server.h>
#include "resources.h"

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

#define PORT                 80            // The TCP port to use
#define MAX_CLIENTS          3

int ledPin = PA15;
int visit_count = 0;

void info_html_generator (const char* url, const char* query, void* http_request);
void file_not_found_generator (const char* url, const char* query, void* http_request);

HTTPPage pages[] = 
{
  HTTPPageRedirect("/", "/index.html"), // redirect root to index.html
  HTTPPage("/404.html"          , HTTP_MIME_TEXT_HTML       , file_not_found_generator ),

  HTTPPage("/32px.png"          , HTTP_MIME_IMAGE_PNG       , &_32px_png         ),
  HTTPPage("/adafruit_logo.png" , HTTP_MIME_IMAGE_PNG       , &adafruit_logo_png ),
  HTTPPage("/favicon.ico"       , HTTP_MIME_IMAGE_MICROSOFT , &favicon_ico       ),
  HTTPPage("/file_sprite.png"   , HTTP_MIME_IMAGE_PNG       , &file_sprite_png   ),
  HTTPPage("/index.html"        , HTTP_MIME_TEXT_HTML       , &index_html        ),

};

uint8_t pagecount = sizeof(pages)/sizeof(HTTPPage);

// Declare HTTPServer with max number of pages
AdafruitHTTPServer httpserver(pagecount);

void info_html_generator (const char* url, const char* query, void* http_request)
{
  (void) url;
  (void) query;
  (void) http_request;

  httpserver.print("<b>Bootloader</b> : ");
  httpserver.print( Feather.bootloaderVersion() );
  httpserver.print("<br>");

  httpserver.print("<b>WICED SDK</b> : ");
  httpserver.print( Feather.sdkVersion() );
  httpserver.print("<br>");

  httpserver.print("<b>FeatherLib</b> : ");
  httpserver.print( Feather.firmwareVersion() );
  httpserver.print("<br>");

  httpserver.print("<b>Arduino API</b> : "); 
  httpserver.print( Feather.arduinoVersion() );
  httpserver.print("<br>");
  httpserver.print("<br>");

  visit_count++;
  httpserver.print("<b>visit count</b> : ");
  httpserver.print(visit_count);
}

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
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the USB serial to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("Simple HTTP Server Example\r\n");
  
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
