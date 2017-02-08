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

/* This example uses the AdafruitHTTPServer class to create a simple 
 * webserver
 */
 
#include <adafruit_feather.h>
#include <adafruit_featherap.h>
#include <adafruit_http_server.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPassword"
#define WLAN_ENCRYPTION       ENC_TYPE_WPA2_AES
#define WLAN_CHANNEL          1

#define PORT                 80            // The TCP port to use
#define MAX_CLIENTS          3

IPAddress apIP     (192, 168, 2, 1);
IPAddress apGateway(192, 168, 2, 1);
IPAddress apNetmask(255, 255, 255, 0);

int ledPin = PA15;
int visit_count = 0;

void info_html_generator      (const char* url, const char* query, httppage_request_t* http_request);
void file_not_found_generator (const char* url, const char* query, httppage_request_t* http_request);

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
AdafruitHTTPServer httpserver(pagecount, WIFI_INTERFACE_SOFTAP);

/**************************************************************************/
/*!
 * @brief  Example of generating dynamic HTML content on demand
 *
 * Link is separated to url and query
 *
 * @param url           url of this page
 * @param query         query string after '?' e.g "var=value"
 *
 * @param http_request  This request's information
*/
/**************************************************************************/
void info_html_generator (const char* url, const char* query, httppage_request_t* http_request)
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

/**************************************************************************/
/*!
 * @brief  HTTP Page 404 generator, HTTP Server will automatically response
 * with page "/404.html" when it coulnd't found requested url in Registered Pages
 *
 * Link is separated to url and query
 *
 * @param url           url of this page
 * @param query         query string after '?' e.g "var=value"
 *
 * @param http_request  This request's information
*/
/**************************************************************************/
void file_not_found_generator (const char* url, const char* query, httppage_request_t* http_request)
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

  // Wait for the Serial Monitor to open
  while (!Serial)
  {
    /* Delay required to avoid RTOS task switching problems */
    delay(1);
  }

  Serial.println("SoftAP Simple HTTP Server Example\r\n");
  
  // Print all software versions
  Feather.printVersions();

  Serial.println("Configuring SoftAP\r\n");
  FeatherAP.err_actions(true, true);
  FeatherAP.setJoinCallback(softap_join_callback);
  FeatherAP.setLeaveCallback(softap_leave_callback);
  FeatherAP.begin(apIP, apGateway, apNetmask, WLAN_CHANNEL);

  Serial.println("Starting SoftAP\r\n");
  FeatherAP.start(WLAN_SSID, WLAN_PASS, WLAN_ENCRYPTION);
  FeatherAP.printNetwork();

  // Tell the HTTP client to auto print error codes and halt on errors
  httpserver.err_actions(true, true);

  // Configure HTTP Server Pages
  Serial.println("Adding Pages to HTTP Server");
  httpserver.addPages(pages, pagecount);
  httpserver.setConnectCallback(httpd_connect_callback);
  httpserver.setDisconnectCallback(httpd_disconnect_callback);

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
    @brief  HTTP Server connect callback
*/
/**************************************************************************/
void httpd_connect_callback(uint32_t remote_ip, uint16_t remote_port)
{
  IPAddress ipaddr(remote_ip);
  Serial.print("HTTPD Connect   : client IP = ");
  Serial.print(ipaddr);
  Serial.print(" port = ");
  Serial.println(remote_port);
}

/**************************************************************************/
/*!
    @brief  HTTP Server disconnect callback
*/
/**************************************************************************/
void httpd_disconnect_callback(uint32_t remote_ip, uint16_t remote_port)
{
  IPAddress ipaddr(remote_ip);
  Serial.print("HTTPD Disconnect: client IP = ");
  Serial.print(ipaddr);
  Serial.print(" port = ");
  Serial.println(remote_port);;    
}

/**************************************************************************/
/*!
    @brief  SoftAP Join callback
*/
/**************************************************************************/
void softap_join_callback(const uint8_t mac[6], uint32_t ipv4)
{
  IPAddress ipaddr(ipv4);
  Serial.print("SoftAP joined: ");
  Serial.println(ipaddr);
}

/**************************************************************************/
/*!
    @brief  SoftAP Leave callback
*/
/**************************************************************************/
void softap_leave_callback(const uint8_t mac[6], uint32_t ipv4)
{
  IPAddress ipaddr(ipv4);
  Serial.print("SoftAP left  : ");
  Serial.println(ipaddr);
}



