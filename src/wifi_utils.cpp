#include "wifi_utils.h"

Ticker ticker;
AsyncWiFiManager wifiManager(&server,&dns);
AsyncWebServer server(80);
DNSServer dns;
WiFiClient client;

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

void configModeCallback (AsyncWiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}



void printWiFiStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi Status: Connected, Network Name: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi Status: Not Connected");
  }
}
