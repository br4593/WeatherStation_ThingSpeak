#include "main_utils.h"


void setup() {
    statusLed();
  Serial.begin(115200);
  ThingSpeak.begin(client);
  pinMode(LED_PIN, OUTPUT);
  addNetwork("rodoy","0544543538");
  addNetwork("BeSpot592F_2.4","9301592F");
  addMultiWifi(networks,numNetworks);
  scanWifiNetworks();
  connectToWifi(); // Connect to WiFi network
  setI2C();

  initSensors();
  temperature = readTemperature();
  humidity = readHumidity();
  pressure = readPressure();
  wind_speed = readWindSpeed();
  wind_direction = readWindDir();
  startWebServer();
  setupTime();
  sensors_flag = true;
  wind_dir_flag = true;
  wind_speed_flag = true;

  delay(5000);
  uploadData();

  //ArduinoOTA.begin();
}

void loop() {
  readSensorData(temperature, humidity, pressure);
  readWindData(wind_direction ,wind_speed);
  //ArduinoOTA.handle();
  statusLed();


if (WiFi.status() != WL_CONNECTED)
{
  connectToWifi();
}
   uploadData();
    /*if (!wifi_flag) {
      connectToWifi();
      if (wifi_flag) {
        uploadData();
        disconnectFromWiFi();
      }
    } else {
      uploadData();
      disconnectFromWiFi();
    }*/

  

  if (millis() - last_serial_print_time >= serial_print_interval) {
    last_serial_print_time = millis();
    printSensorData(temperature, humidity, pressure, wind_speed, wind_direction);
    printWiFiStatus();
  }
}