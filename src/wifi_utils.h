#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>   
#include <Ticker.h>
#include <WiFi.h>

extern Ticker ticker;

extern AsyncWiFiManager wifiManager;

extern AsyncWebServer server;
extern DNSServer dns;

extern WiFiClient client;

void configModeCallback (AsyncWiFiManager *myWiFiManager);
void tick();
void printWiFiStatus();
