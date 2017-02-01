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

/* This example uses the AdafruitHTTPServer class to create a web server 
 * whose resource files are stored in the the SPI flash. The sketch assumes 
 * the SPI flash is enabled by soldering the optional solder jumper for 
 * SPI flash 'closed'.
 * 
 * To run the demo copy the resources folder into SPI Flash's root directory
 */

#include <adafruit_feather.h>
#include <adafruit_http_server.h>
#include <adafruit_spiflash.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

// The TCP port to use
#define PORT                 80

/* Modern browsers uses parallel loading technique which could
 * open up to 6 or 8 connections to render an html page.
 * Increase the MAX_CLIENTS if you often got the httpserver timeout
 */
#define MAX_CLIENTS          8

int ledPin = PA15;

void about_generator (const char* url, const char* query, httppage_request_t* http_request);

HTTPPage pages[] = 
{
  // Page redirections
  HTTPPageRedirect("/", "/index.html"), // Redirect root to '/index.html'

  // Static content
  HTTPPage("/404.html"                        , HTTP_MIME_TEXT_HTML       , "/resources/err404.html"                      ),
  HTTPPage("/apple-touch-icon.png"            , HTTP_MIME_IMAGE_PNG       , "/resources/apple-touch-icon.png"             ),
  HTTPPage("/browserconfig.xml"               , HTTP_MIME_TEXT_XML        , "/resources/browserconfig.xml"                ),
  HTTPPage("/crossdomain.xml"                 , HTTP_MIME_TEXT_XML        , "/resources/crossdomain.xml"                  ),
  HTTPPage("/favicon.ico"                     , HTTP_MIME_IMAGE_MICROSOFT , "/resources/favicon.ico"                      ),
  HTTPPage("/index.html"                      , HTTP_MIME_TEXT_HTML       , "/resources/index.html"                       ),
  HTTPPage("/tile-wide.png"                   , HTTP_MIME_IMAGE_PNG       , "/resources/tile-wide.png"                    ),
  HTTPPage("/tile.png"                        , HTTP_MIME_IMAGE_PNG       , "/resources/tile.png"                         ),
  HTTPPage("/css/main.css"                    , HTTP_MIME_TEXT_CSS        , "/resources/css/main.css"                     ),
  HTTPPage("/css/normalize.css"               , HTTP_MIME_TEXT_CSS        , "/resources/css/normalize.css"                ),
  HTTPPage("/js/main.js"                      , HTTP_MIME_JAVASCRIPT      , "/resources/js/main.js"                       ),
  HTTPPage("/js/plugins.js"                   , HTTP_MIME_JAVASCRIPT      , "/resources/js/plugins.js"                    ),
  HTTPPage("/js/vendor/jquery-1.12.0.min.js"  , HTTP_MIME_JAVASCRIPT      , "/resources/js/vendor/jquery-1.12.0.min.js"   ),
  HTTPPage("/js/vendor/modernizr-2.8.3.min.js", HTTP_MIME_JAVASCRIPT      , "/resources/js/vendor/modernizr-2.8.3.min.js" ),

  // Dynamic content
  HTTPPage("/about.html", HTTP_MIME_TEXT_HTML, about_generator),
};

uint8_t pagecount = sizeof(pages)/sizeof(HTTPPage);

// Declare HTTPServer with max number of pages
AdafruitHTTPServer httpserver(pagecount);

int visit_count = 0;

/**************************************************************************/
/*!
 * @brief  Example of generating dynamic HTML content on demand
 * Link is separated to url and query
 *
 * @param url           url of this page
 * @param query         query string after '?' e.g "var=value"
 *
 * @param http_request  This request's information
*/
/**************************************************************************/
void about_generator (const char* url, const char* query, httppage_request_t* http_request)
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

  // Wait for the Serial Monitor to open
  while (!Serial)
  {
    /* Delay required to avoid RTOS task switching problems */
    delay(1);
  }

  Serial.println("HTTP Server Boilerplat on SPI Flash Example\r\n");
  
  // Print all software versions
  Feather.printVersions();

  // Try to connect to an AP
  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Init SPI Flash
  SpiFlash.begin();

  // Tell the HTTP client to auto print error codes and halt on errors
  httpserver.err_actions(true, true);

  // Configure HTTP Server Pages
  Serial.println("Adding Pages to HTTP Server");
  httpserver.addPages(pages, pagecount);

  Serial.print("Starting HTTP Server ... ");
  if ( !httpserver.begin(PORT, MAX_CLIENTS) )
  {
    Serial.println("Failed");
    Serial.println("Could not start HTTP Server, Please check your Pages config.");
    Serial.println("If resources files are stored in SPI Flash, make sure SpiFlash.begin() is called");
    while(1) 
    {
      delay(1);
    }
  }
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
    Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errnum());
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
