#include "wifi_utils.h"



DNSServer dns;
WiFiClient client;
WiFiManager wm;
WebServer server(80);


bool buttonState = LOW;   // Current state of the button
bool lastButtonState = LOW; // Previous state of the button
bool buttonPressed = false; // Flag to track if the button is pressed
int timeout = 120; // seconds to run for


unsigned long ota_progress_millis = 0;
   bool portalRunning = false;

void flashBuiltInLed()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}


void flashGreenLed()
{
  greenLedState = !greenLedState;

  // Set the LED brightness based on the state
  if (greenLedState) {
    analogWrite(GREEN_LED_PIN, greenLedBrightness); // Full brightness (ON)
  } else {
    analogWrite(GREEN_LED_PIN, 0);   // Off
  }
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

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

/*void checkButton()
{
  buttonState = digitalRead(TRIGGER_PIN);
   // Check if the button was pressed
  if (buttonState == LOW && lastButtonState == HIGH) {
   buttonPressed = true;
  }

  // Check if the button was released
  if (buttonState == HIGH && lastButtonState == LOW) {
   buttonPressed = false;
  }

  if (buttonPressed) {
   wifiManager.setConfigPortalTimeout(timeout);

   if (!wifiManager.startConfigPortal(WIFI_CONFIG_AP,WIFI_CONFIG_PASS)) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
   }


   //if you get here you have connected to the WiFi
   Serial.println("connected...yeey :)");
  }

  lastButtonState = buttonState;

  // Debug print
  Serial.print("Button State: ");
  Serial.println(buttonState);
}*/
void checkButton(){
  // is auto timeout portal running
  if(portalRunning){
    wm.process();
  }

  // is configuration portal requested?
  if(digitalRead(TRIGGER_PIN) == LOW) {
    delay(50);
    if(digitalRead(TRIGGER_PIN) == LOW) {
      if(!portalRunning){
        Serial.println("Button Pressed, Starting Portal");
        wm.startWebPortal();
        portalRunning = true;
      }
      else{
        Serial.println("Button Pressed, Stopping Portal");
        wm.stopWebPortal();
        portalRunning = false;
      }
    }
  }
}
