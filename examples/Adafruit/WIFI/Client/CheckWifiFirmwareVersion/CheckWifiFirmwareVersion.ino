/*
 * This example check if the firmware loaded on the WiFiFeather
 * shield is updated.
 *
 * Circuit:
 * - WiFiFeather Shield attached
 *
 * Created 29 July 2015 by Cristian Maglie
 * This code is in the public domain.
 */
#include <SPI.h>
#include <WiFiFeather.h>

void setup() {
  // Initialize serial
  Serial.begin(9600);
  while (!Serial) {
    delay(1); // wait for serial port to connect. Needed for native USB port only
  }

  // Print a welcome message
  Serial.println("WiFiFeather firmware check.");
  Serial.println();

  // Print firmware version on the shield
  String fv = WiFi.firmwareVersion();
  Serial.print("Firmware version installed: ");
  Serial.println(fv);

  // Print required firmware version
  Serial.print("Firmware version required : ");
  Serial.println(WIFI_FIRMWARE_REQUIRED);

  // Check if the required version is installed
  Serial.println();
  if (fv == WIFI_FIRMWARE_REQUIRED) {
    Serial.println("Check result: PASSED");
  } else {
    Serial.println("Check result: NOT PASSED");
    Serial.println(" - The firmware version on the shield do not match the");
    Serial.println("   version required by the library, you may experience");
    Serial.println("   issues or failures.");
  }
}

void loop() {
  // do nothing
}

