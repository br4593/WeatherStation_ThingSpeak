#include "main_utils.h"


void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Booting");

 

  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);

  // Attach ticker to flash built-in LED
  ticker.attach(0.6, flashBuiltInLed);

  // Set authentication credentials for OTA updates
  ElegantOTA.setAuth(OTA_USER, OTA_PASS);

   /*// Set WiFi mode to station mode
  WiFi.mode(WIFI_STA);*/
  wm.setConnectTimeout(60);
  wm.autoConnect(WIFI_CONFIG_AP,WIFI_CONFIG_PASS);
  
  delay(100);

  // Set pin modes for trigger, green LED, and red LED
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

 

  // Delay for stability
  delay(1000);

  // Turn on built-in LED and red LED
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);

  // Set I2C pins
  Serial.println("Setting up I2C pins...");
  setI2C();

  // Delay for stability
  delay(500);

  // Initialize all I2C sensors
  Serial.println("Initializing sensors...");
  initSensors();

   // Initialize ThingSpeak client
  ThingSpeak.begin(client);

  // Delay for stability
  delay(1000);

  // Initial reading of all sensor values upon startup
  if (checkForSensorsError()) {
    printError(errorInfo);
  } else {
    Serial.println("Reading sensor data...");
    temperature = readTemperature();
    humidity = readHumidity();
    pressure = readPressure();
    wind_speed = readWindSpeed();
    wind_direction = readWindDir();
  }
  

  // Define root route for HTTP server
  Serial.println("Setting up HTTP server...");
  server.on("/", []() {
    server.send(200, "text/plain", "Hi! This is ElegantOTA Demo.");
  });

  // Start ElegantOTA
  Serial.println("Starting OTA server...");
  ElegantOTA.begin(&server);

  // Start HTTP server
  Serial.println("Starting HTTP server...");
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
  sensors_flag = true;
  wind_dir_flag = true;
  wind_speed_flag = true;

  // Delay for stability
  delay(1000);

  

  // Upload data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wifi connected");
    uploadData();
    delay(15000);
    currentRainfall = ThingSpeak.readFloatField(TS_CH,RAIN_CH,READ_TS_API_KEY);
  }

  

}

void loop() {
  //Serial.println("Looping...");
  //checkButton();

  // Delay for stability
  delay(100);

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

//spd_voltage_debug = ads.computeVolts(ads.readADC_SingleEnded(1));
//dir_voltage_debug = ads.computeVolts(ads.readADC_SingleEnded(WIND_DIR_SENSOR_ADC_CHANNEL));

  // Print formatted time
  //Serial.println(timeClient.getFormattedTime());

  // Delay for stability
  delay(1000);



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
  if (millis() - last_serial_print_time >= serial_print_interval) {
    last_serial_print_time = millis();
    printSensorData(temperature, humidity, pressure, wind_speed, wind_direction);
    printWiFiStatus();
  }
}
