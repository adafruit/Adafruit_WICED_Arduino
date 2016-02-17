#include <adafruit_feather.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPass"

byte server[] = { 64, 233, 187, 99 }; // Google

AdafruitTCP client;

void setup()
{
  Serial.begin(9600);
  while(!Serial) delay(1);

  delay(1000);

  // Attempt to connect to an AP
  Serial.print("Attempting to connect to: ");
  Serial.println(WLAN_SSID);
  
  if ( feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println("connecting...");
  
    if (client.connect(server, 80)) {
      Serial.println("connected");
      client.println("GET /search?q=arduino HTTP/1.0");
      client.println();
    } else {
      Serial.println("connection failed");
    }
  }
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}
