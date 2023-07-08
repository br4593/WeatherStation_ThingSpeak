#include "main_utils.h"

// Error flags for sensors
bool ads1115_error = false;
bool sht31_error = false;
bool bme280_error = false;

WiFiClient client;

// Variables to track the last reading time
unsigned long last_serial_print_time = 0;
unsigned long last_wifi_status_time = 0;
unsigned long last_sensors_reading_time = 0;
unsigned long last_wind_sample_time = 0;
const unsigned long serial_print_interval = 60000; // Print sensor data every 1 minute
const unsigned long wifi_status_interval = 30000; // Print WiFi status every 1 minute

// Flags to indicate which sensor data to read
bool sensors_flag = false;
bool wind_speed_flag = false;
bool wind_dir_flag = false;
bool wifi_flag = false;

float temperature = 0;
float humidity = 0;
float pressure = 0;
float wind_speed = 0;
int wind_direction = 0;

unsigned long myChannelNumber = 2214506;
const char* myWriteAPIKey = "WRDZS4RBBRZBSI6E";

/**
 * Reads sensor data from various sensors.
 *
 * @param temperature [out] - The variable to store the temperature reading.
 * @param humidity [out] - The variable to store the humidity reading.
 * @param pressure [out] - The variable to store the pressure reading.
 * @param wind_speed [out] - The variable to store the wind speed reading.
 * @param wind_direction [out] - The variable to store the wind direction reading.
 */
void readSensorData(float& temperature, float& humidity, float& pressure, float& wind_speed, int& wind_direction) {
  if (millis() - last_sensors_reading_time >= SENSOR_READING_INTERVAL) {
    // Read temperature and humidity from SHT31 sensor
    temperature = readTemperature();
    humidity = readHumidity();
    pressure = readPressure();
    printSensorData(temperature, humidity, pressure, wind_speed, wind_direction);
    last_sensors_reading_time = millis();
    sensors_flag = true;
  }

  // Read wind speed from ADS1115 analog-to-digital converter
  int16_t wind_speed_adc_ch_reading = ads.readADC_SingleEnded(WIND_SPEED_SENSOR_ADC_CH);
  float wind_speed_volts = ads.computeVolts(wind_speed_adc_ch_reading);
  float temp_wind_speed = sampleWindSpeed(wind_speed_volts, aveWindSpdCalcArr);

  if (wind_speed_flag) {
    wind_speed = temp_wind_speed;
    wind_speed_flag = false;
  }

  // Read wind direction from ADS1115 analog-to-digital converter
  int16_t wind_dir_adc_ch_reading = ads.readADC_SingleEnded(WIND_DIR_SENSOR_ADC_CH);
  float wind_dir_volts = ads.computeVolts(wind_dir_adc_ch_reading);
  int temp_wind_direction = sampleWindDirection(wind_dir_volts, aveWindDirCalcArr);

  if (wind_dir_flag) {
    wind_direction = temp_wind_direction;
    wind_dir_flag = false;
  }
}

/**
 * Prints the sensor data to the Serial monitor.
 *
 * @param temperature - The temperature reading in degrees Celsius.
 * @param humidity - The humidity reading in percentage.
 * @param pressure - The pressure reading in hectopascals (hPa).
 * @param wind_speed - The wind speed reading in kilometers per hour (km/h).
 * @param wind_direction - The wind direction as a string.
 */
void printSensorData(float temperature, float humidity, float pressure, float wind_speed, int& wind_direction) {
  // Print current local time
  printLocalTime();

  // Print temperature, humidity, and pressure readings
  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity, 1);
  Serial.print("%, Pressure: ");
  Serial.print(pressure, 2);
  Serial.println(" hPa");

  // Print wind speed and direction readings
  Serial.print("Wind Speed: ");
  Serial.print(wind_speed);
  Serial.println(" km/h");

  Serial.print("Wind Direction: ");
  Serial.println(wind_direction);
  Serial.println("--------------------");
}

/**
 * Initializes the sensors used in the system.
 */
void initSensors() {
  // Setup ADS1115 analog-to-digital converter
  setupADS1115();

  // Setup BME280 temperature, pressure, and humidity sensor
  setupBME280();

  // Setup SHT31 temperature and humidity sensor
  setupSHT31();
}

/**
 * Resets the necessary Arduino Cloud variables.
 */
void resetArduinoCloudVariables() {
  // Reset Arduino Cloud variables
  // TODO: Add code to reset the necessary Arduino Cloud variables
}

/**
 * Sets up I2C communication using SDA and SCL pin definitions.
 */
void setI2C() {
  // Set up I2C communication using SDA and SCL pin definitions
  Wire.begin(SDA, SCL);
  Wire.setClock(100000);
}

/**
 * Prints error information.
 *
 * @param errorInfo - The ErrorInfo struct containing error flags.
 */
void printError(const ErrorInfo& errorInfo) {
  // Print error information
  if (errorInfo.ads1115) {
    Serial.println("Error: ADS1115 component error!");
  }
  if (errorInfo.sht31) {
    Serial.println("Error: SHT31 component error!");
  }
  if (errorInfo.bme280) {
    Serial.println("Error: BME280 component error!");
  }
}

/**
 * Calculates the vapor pressure deficit (VPD) given temperature and humidity.
 *
 * @param t - The temperature in degrees Celsius.
 * @param h - The humidity in percentage.
 * @return The vapor pressure deficit.
 */
float cal_vpd(float t, float h) {
  float es = exp(((16.78 * t) - 116.9) / (t + 237.3));
  float ed = es * (h / 100);
  float vpd = es - ed;

  return vpd;
}

/**
 * Connects to the WiFi network.
 *
 * @note Replace "your_SSID" and "your_password" with your WiFi network credentials.
 */
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(NETWORK, PASSWORD); // Replace with your WiFi SSID and password
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    wifi_flag = true; // WiFi is connected, set the flag to true
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

/**
 * Reconnects to the WiFi network.
 */
void reconnectToWiFi() {
  Serial.println("Reconnecting to WiFi...");
  WiFi.disconnect(); // Disconnect from WiFi
  WiFi.reconnect(); // Reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    wifi_flag = false; // Set the flag to false if WiFi reconnection fails
    Serial.println("WiFi not connected!");
    delay(1000);
  } else {
    wifi_flag = true;
    Serial.println("WiFi reconnected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

/**
 * Uploads sensor data to ThingSpeak.
 */
void uploadData() {
  printSensorData(temperature, humidity, pressure, wind_speed, wind_direction);
  ThingSpeak.setField(4, humidity);
  ThingSpeak.setField(3, temperature);
  ThingSpeak.setField(5, pressure);
  ThingSpeak.setField(1, wind_speed);
  ThingSpeak.setField(2, wind_direction);
  ThingSpeak.setField(6, cal_vpd(temperature, humidity));
  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (response == 200) {
    Serial.println("Data uploaded to ThingSpeak server successfully.");
  } else {
    Serial.print("Error uploading data to ThingSpeak. HTTP error code: ");
    Serial.println(response);
  }
}


/**
 * Disconnects from the WiFi network.
 */
void disconnectFromWiFi() {
  WiFi.disconnect();
  wifi_flag = false;
  Serial.println("WiFi disconnected");
}

void printWiFiStatus() {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("WiFi Status: Connected, Network Name: ");
      Serial.println(WiFi.SSID());
    } else {
      Serial.println("WiFi Status: Not Connected");
    }
  }

