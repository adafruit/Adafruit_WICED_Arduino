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

#define WLAN_SSID            "your_ssid"
#define WLAN_PASS            "your_password"

#define PORT                 80            // The TCP port to use
#define MAX_CLIENTS          5

int ledPin = PA15;

void about_generator (const char* url, const char* query, void* http_request);

// Page database
HTTPPage pages[] =
{
  // Page redirections
  HTTPPageRedirect("/", "/index.html"), // Redirect root to '/index.html'

  // Static content
  HTTPPage("/404.html"                        , HTTP_MIME_TEXT_HTML       , &err404_html), // Note: files can't start with a number!
  HTTPPage("/apple-touch-icon.png"            , HTTP_MIME_IMAGE_PNG       , &apple_touch_icon_png),
  HTTPPage("/browserconfig.xml"               , HTTP_MIME_TEXT_XML        , &browserconfig_xml),
  HTTPPage("/crossdomain.xml"                 , HTTP_MIME_TEXT_XML        , &crossdomain_xml),
  HTTPPage("/favicon.ico"                     , HTTP_MIME_IMAGE_MICROSOFT , &favicon_ico),
  HTTPPage("/index.html"                      , HTTP_MIME_TEXT_HTML       , &index_html),
  HTTPPage("/tile-wide.png"                   , HTTP_MIME_IMAGE_PNG       , &tile_wide_png),
  HTTPPage("/tile.png"                        , HTTP_MIME_IMAGE_PNG       , &tile_png),
  HTTPPage("/css/main.css"                    , HTTP_MIME_TEXT_CSS        , &css_dir_main_css),
  HTTPPage("/css/normalize.css"               , HTTP_MIME_TEXT_CSS        , &css_dir_normalize_css),
  HTTPPage("/js/main.js"                      , HTTP_MIME_JAVASCRIPT      , &js_dir_main_js),
  HTTPPage("/js/plugins.js"                   , HTTP_MIME_JAVASCRIPT      , &js_dir_plugins_js),
  HTTPPage("/js/vendor/jquery-1.12.0.min.js"  , HTTP_MIME_JAVASCRIPT      , &js_dir_vendor_dir_jquery_1_12_0_min_js),
  HTTPPage("/js/vendor/modernizr-2.8.3.min.js", HTTP_MIME_JAVASCRIPT      , &js_dir_vendor_dir_modernizr_2_8_3_min_js),

  // Dynamic content
  HTTPPage("/about.html", HTTP_MIME_TEXT_HTML, about_generator),
};

uint8_t pagecount = sizeof(pages)/sizeof(HTTPPage);

// Use the HTTP class
AdafruitHTTPServer httpserver(pagecount);

uint32_t visit_count = 0;

/**************************************************************************/
/*!
    @brief  Example of generating dynamic HTML content on demand
*/
/**************************************************************************/
void about_generator (const char* url, const char* query, void* http_request)
{
  (void) url;
  (void) query;
  (void) http_request;

  visit_count++;

  int heap_used_size = Feather.dbgHeapUsed();
  int heap_free_size = Feather.dbgHeapFree();

  // Note that all double quotes have been replaced with single quotes in
  // the HTML code below, taken from the default index.html template file.
  // Some double quotes and backslashes have also been escaped ('\' to '\\',
  // '"' to '\"') where the character needing an escape code couldn't be
  // avoided.

  httpserver.print("<!doctype html>\n"
                   "<html class='no-js' lang=''>\n"
                   "    <head>\n"
                   "        <meta charset='utf-8'>\n"
                   "        <meta http-equiv='x-ua-compatible' content='ie=edge'>\n"
                   "        <title></title>\n"
                   "        <meta name='description' content=''>\n"
                   "        <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
                   "        <link rel='apple-touch-icon' href='apple-touch-icon.png'>\n"
                   "        <link rel='stylesheet' href='css/normalize.css'>\n"
                   "        <link rel='stylesheet' href='css/main.css'>\n"
                   "        <script src='js/vendor/modernizr-2.8.3.min.js'></script>\n"
                   "    </head>\n"
                   "    <body>\n"
                   "        <!--[if lt IE 8]>\n"
                   "            <p class='browserupgrade'>You are using an <strong>outdated</strong> browser. Please <a href='http://browsehappy.com/'>upgrade your browser</a> to improve your experience.</p>\n"
                   "        <![endif]-->\n");

  // Insert the dynamic body content here
  httpserver.print("        <h2>Module Details</h2>\n");
  httpserver.print("        <ul>\n");
  httpserver.print("            <li><b>Bootloader</b> : "); httpserver.print( Feather.bootloaderVersion() ); httpserver.print("</li>\n");
  httpserver.print("            <li><b>WICED SDK</b> : "); httpserver.print( Feather.sdkVersion() ); httpserver.print("</li>\n");
  httpserver.print("            <li><b>FeatherLib</b> : "); httpserver.print( Feather.firmwareVersion() ); httpserver.print("</li>\n");
  httpserver.print("            <li><b>Arduino API</b> : "); httpserver.print( Feather.arduinoVersion() ); httpserver.print("</li>\n");
  httpserver.print("            <li><b>Heap Used</b> : "); httpserver.print( heap_used_size ); httpserver.print(" bytes</li>\n");
  httpserver.print("            <li><b>Heap Free</b> : "); httpserver.print( heap_free_size ); httpserver.print(" bytes</li>\n");
  httpserver.print("        </ul>\n");
  httpserver.print("        <h2>Page Details</h2>\n");
  httpserver.print("        <ul>\n");
  httpserver.print("            <li><b>Page Loads</b> : "); httpserver.print( visit_count ); httpserver.print("</li>\n");
  httpserver.print("        </ul>\n");
  httpserver.print("        <p>This page has been dynamically generated.</p>\n");

  // Take note of the escaped backslash and double-quotes in the text below
  httpserver.print("        <script src='https://code.jquery.com/jquery-1.12.0.min.js'></script>\n"
                   "        <script>window.jQuery || document.write('<script src=\"js/vendor/jquery-1.12.0.min.js\"><\\/script>')</script>\n"
                   "        <script src='js/plugins.js'></script>\n"
                   "        <script src='js/main.js'></script>\n"
                   "        <!-- Google Analytics: change UA-XXXXX-X to be your site's ID. -->\n"
                   "        <script>\n"
                   "            (function(b,o,i,l,e,r){b.GoogleAnalyticsObject=l;b[l]||(b[l]=\n"
                   "            function(){(b[l].q=b[l].q||[]).push(arguments)});b[l].l=+new Date;\n"
                   "            e=o.createElement(i);r=o.getElementsByTagName(i)[0];\n"
                   "            e.src='https://www.google-analytics.com/analytics.js';\n"
                   "            r.parentNode.insertBefore(e,r)}(window,document,'script','ga'));\n"
                   "            ga('create','UA-XXXXX-X','auto');ga('send','pageview');\n"
                   "        </script>\n"
                   "    </body>\n"
                   "</html>\n");
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
