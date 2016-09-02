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

/* This example use HTML Boilerplate 5.3.0 on the HTTP server to illustrate 
 *  how to create a flexible web site on your WICED Feather.
 *  
 * The raw source files used in this demo are included in resources folder.
 * 
 * To convert the source files (html, jpg etc..) into C headers, please use
 * the included 'tools/pyresource' script:
 *
 *   $ python ../../../tools/pyresource/pyresource.py resources
 * 
 * pyresource will convert each file to a header file, and '.' will be
 * replaced by '_' in the header file name(s). For example:
 * 
 *   index.html --> index_html.h with HTTPResource variable name 'index_html'
 *    
 * For your convenience, a master header named resources.h is also generated 
 * which includes all of generated header files. Your sketch simply needs to
 * include a reference to this file:
 * 
 *   #include "resources.h"
 */

#include <adafruit_feather.h>
#include <adafruit_http_server.h>
#include "resources.h"

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

#define PORT                 80            // The TCP port to use
#define MAX_CLIENTS          3

int ledPin = PA15;

void heap_generator (const char* url, const char* query, void* http_request);
void thread_generator (const char* url, const char* query, void* http_request);

// Page database
HTTPPage pages[] = 
{
  HTTPPageRedirect("/", "/index.html"), // Redirect root requests to '/index.html'

  HTTPPage("/404.html"              , HTTP_MIME_TEXT_HTML           , &err404_html), // Note: files can't start with a number!
  HTTPPage("/apple-touch-icon.png"  , HTTP_MIME_IMAGE_PNG           , &apple_touch_icon_png),
  HTTPPage("/browserconfig.xml"     , HTTP_MIME_TEXT_PLAIN          , &browserconfig_xml),
  HTTPPage("/crossdomain.xml"       , HTTP_MIME_TEXT_PLAIN          , &crossdomain_xml),
  HTTPPage("/favicon.ico"           , HTTP_MIME_IMAGE_MICROSOFT     , &favicon_ico),
  HTTPPage("/index.html"            , HTTP_MIME_TEXT_HTML           , &index_html),
  HTTPPage("/robots.txt"            , HTTP_MIME_TEXT_PLAIN          , &robots_txt),
  HTTPPage("/tile-wide.png"         , HTTP_MIME_IMAGE_PNG           , &tile_wide_png),
  HTTPPage("/tile.png"              , HTTP_MIME_IMAGE_PNG           , &tile_png),

  HTTPPage("/css/main.css"          , HTTP_MIME_TEXT_CSS            , &main_css),
  HTTPPage("/css/normalize.css"     , HTTP_MIME_TEXT_CSS            , &normalize_css),

  HTTPPage("/js/main.js"            , HTTP_MIME_JAVASCRIPT          , &main_js),
  HTTPPage("/js/plugins.js"         , HTTP_MIME_JAVASCRIPT          , &plugins_js),
  HTTPPage("/js/vendor/jquery-1.12.0.min.js", HTTP_MIME_JAVASCRIPT  , &jquery_1_12_0_min_js),
  HTTPPage("/js/vendor/modernizr-2.8.3.min.js", HTTP_MIME_JAVASCRIPT, &modernizr_2_8_3_min_js),

  HTTPPage("/heap.csv"   , HTTP_MIME_TEXT_PLAIN, heap_generator),
  HTTPPage("/thread.csv" , HTTP_MIME_TEXT_PLAIN, thread_generator),
};

uint8_t pagecount = sizeof(pages)/sizeof(HTTPPage);

// Use the HTTP class
AdafruitHTTPServer httpserver(pagecount);

thread_info_t threadInfo[20];

// This dynamic page generate populates a CSV file with heap memory usage details
void heap_generator (const char* url, const char* query, void* http_request)
{
  (void) url;
  (void) query;
  (void) http_request;

  int used_size = Feather.dbgHeapUsed();
  int free_size = Feather.dbgHeapFree();

  httpserver.print("label,count\r\n");
  httpserver.printf("Used %ld,%ld\r\n", used_size, used_size);
  httpserver.printf("Free %ld,%ld\r\n", free_size, free_size);
}

// This dynamic page generate populates a CSV file with info on FeatherLib RTOS threads
void thread_generator (const char* url, const char* query, void* http_request)
{
  (void) url;
  (void) query;
  (void) http_request;

  httpserver.print("Name,Used,Free\r\n");
  
  // Get Thread Info
  int count = Feather.dbgThreadInfo(threadInfo, 20);
  //DBG_INT(count);
  
  for(int i=0; i<count; i++)
  {
    httpserver.printf("%s (%u),%ld,%ld\r\n", 
                      threadInfo[i].name, threadInfo[i].prio,
                      threadInfo[i].stack_highest, threadInfo[i].stack_total-threadInfo[i].stack_highest);
  }
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
