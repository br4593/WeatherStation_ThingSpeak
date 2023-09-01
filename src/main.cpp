#include "main_utils.h"


void setup() {
  Serial.begin(115200);
     wifiManager.setAPCallback(configModeCallback);
  ThingSpeak.begin(client);
    pinMode(LED_PIN, OUTPUT);
      ticker.attach(0.6, tick);
   bool res;

  if (!wifiManager.autoConnect("WeatherStation","11223344")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, HIGH);

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

}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi not connected");
    WiFi.begin();
  }
  readSensorData(temperature, humidity, pressure);
  readWindData(wind_direction ,wind_speed);
  uploadData();


  if (millis() - last_serial_print_time >= serial_print_interval) {
    last_serial_print_time = millis();
    printSensorData(temperature, humidity, pressure, wind_speed, wind_direction);
    printWiFiStatus();
  }
}