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

/* This example use the AdafruitHTTP class to send requests and get
 * the response via a callback */

#include <adafruit_feather.h>
#include <adafruit_http_server.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"

#define PORT                 80            // The TCP port to use
#define MAX_CLIENTS          3

int ledPin = PA15;
int visit_count = 0;

void info_html_generator (const char* url, const char* query, void* http_request);
void file_not_found_generator (const char* url, const char* query, void* http_request);

const char hello_html[] = "<html><body> <h1>Hello World!</h1> </body></html>";

HTTPPage pages[] = 
{
  HTTPPageRedirect("/", "/hello.html"), // redirect root to hello page
  HTTPPage("/hello.html", HTTP_MIME_TEXT_HTML, hello_html),
  HTTPPage("/info.html" , HTTP_MIME_TEXT_HTML, info_html_generator),
  HTTPPage("/404.html" , HTTP_MIME_TEXT_HTML, file_not_found_generator),
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
  httpserver.println( Feather.bootloaderVersion() );

  httpserver.print("<b>WICED SDK</b> : ");
  httpserver.println( Feather.sdkVersion() );

  httpserver.print("<b>FeatherLib</b> : ");
  httpserver.println( Feather.firmwareVersion() );

  httpserver.print("<b>Arduino API</b> : "); 
  httpserver.println( Feather.arduinoVersion() );
  httpserver.println();

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
  httpserver.println("<h1>Error 404 File Not Found!</h1>");
  
  httpserver.println("Available pages are:");
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
