<<<<<<< HEAD

//#include <ESPAsyncWiFiManager.h>   
//extern AsyncWiFiManager wifiManager;
#define TRIGGER_PIN 4

#include "main_utils.h"
#include <WiFiManager.h>
#include <ElegantOTA.h>
#include <Ticker.h>
#include <WiFi.h>



extern WiFiManager wifiManager;
extern DNSServer dns;
extern WiFiClient client;
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
=======

//#include <ESPAsyncWiFiManager.h>   
//extern AsyncWiFiManager wifiManager;
#define TRIGGER_PIN 4

#include "main_utils.h"
#include <WiFiManager.h>
#include <ElegantOTA.h>
#include <Ticker.h>
#include <WiFi.h>



extern WiFiManager wifiManager;
extern DNSServer dns;
extern WiFiClient client;
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
>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98
