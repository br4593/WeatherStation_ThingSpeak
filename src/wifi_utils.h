
//#include <ESPAsyncWiFiManager.h>   
//extern AsyncWiFiManager wifiManager;
#define TRIGGER_PIN 15

#include "main_utils.h"
#include <WiFiManager.h>
#include <ElegantOTA.h>
#include <Ticker.h>
#include <WiFi.h>
//#include <WiFiClientSecure.h>




extern WiFiManager wm;
extern DNSServer dns;
extern WiFiClient client;
//extern WiFiClientSecure client;
extern WebServer server;



extern unsigned long ota_progress_millis;
extern bool portalRunning;

extern bool buttonState; 
extern bool lastButtonState;
extern bool buttonPressed;
extern int timeout;

//void configModeCallback (AsyncWiFiManager *myWiFiManager);
void flashBuiltInLed();
void flashGreenLed();
void printWiFiStatus();
void onOTAStart();
void onOTAProgress(size_t current, size_t final);
void onOTAEnd(bool success);
void checkButton();
