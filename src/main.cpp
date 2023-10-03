#include "main_utils.h"



void setup() {

  Serial.begin(115200);
  ThingSpeak.begin(client);
    pinMode(LED_PIN, OUTPUT);
      ticker.attach(0.6, flashBuiltInLed);
      ElegantOTA.setAuth("barrod4593", "GarminGNS430");

   pinMode(TRIGGER_PIN, INPUT_PULLUP);
   pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP , for WifiManager
  delay(1000);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  digitalWrite(BUILTIN_LED, HIGH);


  setI2C();//Set I2C Pins
  delay(500);

  initSensors();//Init all I2C Sensors

  delay(1000);

/**
 * Initiale reading of all values upon startup
*/
  temperature = readTemperature();
  humidity = readHumidity();
  pressure = readPressure();
  wind_speed = readWindSpeed();
  wind_direction = readWindDir();


  server.on("/", []() {
    server.send(200, "text/plain", "Hi! This is ElegantOTA Demo.");
  });



  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
  debugPage();
  setupTime();
  sensors_flag = true;
  wind_dir_flag = true;
  wind_speed_flag = true;

  delay(1000);
  uploadData();


}

void loop() {
spd_voltage_debug = ads.computeVolts(ads.readADC_SingleEnded(wind_speed_sensor_adc_channel));
delay(500);
dir_voltage_debug = ads.computeVolts(ads.readADC_SingleEnded(1));

server.handleClient();
  ElegantOTA.loop();




checkButton();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi not connected");
    greenLedTicker.attach(0.5,flashGreenLed);
    WiFi.begin();
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    greenLedTicker.detach();
    digitalWrite(GREEN_LED_PIN, HIGH);
  }

  if (checkForSensorsError())
  {
    error_debug = true;
    redLedTicker.attach(0.5,flashRedLed);
    printError(errorInfo);
    delay(500);
  }

  else
  {
    redLedTicker.detach();
    digitalWrite(RED_LED_PIN, LOW);
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