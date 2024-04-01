#include "main_utils.h"

<<<<<<< HEAD
void setup() {
  // Initialize serial communication
=======


void setup() {

>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98
  Serial.begin(115200);
  Serial.println("Booting");

  // Initialize ThingSpeak client
  ThingSpeak.begin(client);
<<<<<<< HEAD

  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);

  // Attach ticker to flash built-in LED
  ticker.attach(0.6, flashBuiltInLed);

  // Set authentication credentials for OTA updates
  ElegantOTA.setAuth("barrod4593", "GarminGNS430");

  // Set pin modes for trigger, green LED, and red LED
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  // Set WiFi mode to station mode
  WiFi.mode(WIFI_STA);

  // Delay for stability
  delay(1000);

  // Turn on built-in LED and red LED
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);

  // Set I2C pins
  setI2C();

  // Delay for stability
  delay(500);

  // Initialize all I2C sensors
  initSensors();

  // Delay for stability
  delay(1000);

  // Initial reading of all sensor values upon startup
=======
    pinMode(LED_PIN, OUTPUT);
      ticker.attach(0.6, flashBuiltInLed);
      ElegantOTA.setAuth("", "");

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
>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98
  temperature = readTemperature();
  humidity = readHumidity();
  pressure = readPressure();
  wind_speed = readWindSpeed();
  wind_direction = readWindDir();

<<<<<<< HEAD
  // Define root route for HTTP server
=======

>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98
  server.on("/", []() {
    server.send(200, "text/plain", "Hi! This is ElegantOTA Demo.");
  });

<<<<<<< HEAD
  // Start ElegantOTA
  ElegantOTA.begin(&server);

  // Start HTTP server
  server.begin();
  Serial.println("HTTP server started");

  // Display debug page
  debugPage();

  // Initialize time client
  timeClient.begin();

  // Delay for stability
  delay(500);

  // Set time offset
  timeClient.setTimeOffset(10800);

  currentDay = timeClient.getDay();

  // Set flags
=======


  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
  debugPage();
  setupTime();
>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98
  sensors_flag = true;
  wind_dir_flag = true;
  wind_speed_flag = true;

<<<<<<< HEAD
  // Delay for stability
  delay(1000);

  // Upload data to ThingSpeak
  uploadData();

  delay(15000);

  // Read current rainfall from ThingSpeak

  currentRainfall = ThingSpeak.readFloatField(TS_CH,RAIN_CH,READ_TS_API_KEY);
}

void loop() {
  // Read wind speed voltage
  spd_voltage_debug = ads.computeVolts(ads.readADC_SingleEnded(wind_speed_sensor_adc_channel));
=======
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
>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98

  // Delay for stability
  delay(500);

<<<<<<< HEAD
  // Read wind direction voltage
  dir_voltage_debug = ads.computeVolts(ads.readADC_SingleEnded(wind_dir_sensor_adc_channel));

  // Handle HTTP client requests
  server.handleClient();

  // Handle ElegantOTA updates
  ElegantOTA.loop();

  // Update time client
  timeClient.update();
  
  if (currentDay != timeClient.getDay())
  {
    currentRainfall = 0;
  }



  // Print formatted time
  Serial.println(timeClient.getFormattedTime());

  // Delay for stability
  delay(1000);

  // Check button state
  checkButton();

  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi not connected");
    greenLedTicker.attach(0.5, flashGreenLed);
    WiFi.begin();
    delay(500);
  }

  // If WiFi is connected, turn off green LED
  if (WiFi.status() == WL_CONNECTED) {
    greenLedTicker.detach();
    digitalWrite(GREEN_LED_PIN, HIGH);
  }

  // Check for sensor errors
  if (checkForSensorsError()) {
    digitalWrite(RED_LED_PIN, LOW);
    error_debug = true;
    redLedTicker.attach(0.5, flashRedLed);
    printError(errorInfo);
    delay(500);

    // Re-setup BME280 if error occurred
    if (errorInfo.bme280) {
      setupBME280();
    }

    // Re-setup SHT31 if error occurred
    if (errorInfo.sht31) {
      setupSHT31();
    }

    // Re-setup ADS1115 if error occurred
    if (errorInfo.ads1115) {
      setupADS1115();
    }
  }

  // If no sensor errors, turn off red LED
  if (!checkForSensorsError()) {
    error_debug = false;
    redLedTicker.detach();
    digitalWrite(RED_LED_PIN, HIGH);
  }

  // Read sensor data
  readSensorData(temperature, humidity, pressure);

  // Read wind data
  readWindData(wind_direction, wind_speed);

  // Upload data to ThingSpeak
  uploadData();

  // Print sensor data and WiFi status at specified interval
=======
>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98
  if (millis() - last_serial_print_time >= serial_print_interval) {
    last_serial_print_time = millis();
    printSensorData(temperature, humidity, pressure, wind_speed, wind_direction);
    printWiFiStatus();
  }
}
