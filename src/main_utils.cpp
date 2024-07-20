#include "main_utils.h"



// Variables to track the last reading time
unsigned long last_serial_print_time = 0;
unsigned long last_wifi_status_time = 0;
unsigned long last_sensors_reading_time = 0;
unsigned long last_wind_sample_time = 0;
const unsigned long serial_print_interval = 60000; // Print sensor data every 1 minute
const unsigned long wifi_status_interval = 30000; 
int latest_http_response = 0;

int SENSORS_READING_INTERVAL_IN_MINUTES = 15;
int SENSORS_READING_INTERVAL = SENSORS_READING_INTERVAL_IN_MINUTES * 60 * 1000;

const unsigned int MINIMUM_UPLOAD_INTERVAL = 25000;
unsigned int previous_upload_time = 0;

// Flags to indicate which sensor data to read
bool sensors_flag = false;
bool wind_speed_flag = false;
bool wind_dir_flag = false;

float temperature = 0;
float humidity = 0;
float pressure = 0;
float wind_speed = 0;
int wind_direction = 0;
float dir_voltage_debug = 0;
float spd_voltage_debug = 0;

int error_debug = 0;


const int LED_PIN = 15; // Replace with the appropriate onboard LED pin for your ESP32 board
const int GREEN_LED_PIN = 19;
const int RED_LED_PIN = 18;
const unsigned long BLINK_INTERVAL = 500; // LED blink interval in milliseconds
unsigned long previousBlinkMillis = 0;

Ticker ticker;
Ticker greenLedTicker;
Ticker redLedTicker;
    ErrorInfo errorInfo = {false, false, false};


const int greenLedBrightness = 80;
bool greenLedState = false;
 const int redLedBrightness = 100;
 bool redLedState = false;


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int currentDay;


/**
 * Reads sensor data from various sensors.
 *
 * @param temperature [out] - The variable to store the temperature reading.
 * @param humidity [out] - The variable to store the humidity reading.
 * @param pressure [out] - The variable to store the pressure reading.
 * @param wind_speed [out] - The variable to store the wind speed reading.
 * @param wind_direction [out] - The variable to store the wind direction reading.
 */
void readSensorData(float& temperature, float& humidity, float& pressure) {
  if (millis() - last_sensors_reading_time >= SENSORS_READING_INTERVAL) {
    // Read temperature and humidity from SHT31 sensor
    temperature = readTemperature();
    humidity = readHumidity();
    pressure = readPressure();
    last_sensors_reading_time = millis();// Update last reading time
    sensors_flag = true;// Set flag to indicate that sensor data needs to be uploaded
  }

}

void readWindData(int& wind_direction, float& wind_speed)
{
    readAverageWinds(wind_speed ,wind_direction);
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
/**
 * Initializes the sensors used in the weather station.
 * This function sets up the ADS1115 analog-to-digital converter,
 * the BME280 temperature, pressure, and humidity sensor,
 * the SHT31 temperature and humidity sensor, and the rain fall sensor.
 */
void initSensors() {
  // Setup ADS1115 analog-to-digital converter
  setupADS1115();

  // Setup BME280 temperature, pressure, and humidity sensor
  setupBME280();

  // Setup SHT31 temperature and humidity sensor
  setupSHT31();

  // Setup rain fall sensor
  setupRainFallSensor();
  
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
void printError(struct ErrorInfo& errorInfo) {
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
 * Uploads sensor data to ThingSpeak.
 */
int uploadData() {
  // Check if sensor data needs to be uploaded
  int response = 0;
  if (sensors_flag) {
    ThingSpeak.setField(HUMIDITY_CH, humidity);
    ThingSpeak.setField(TEMP_CH, temperature);
    ThingSpeak.setField(PRESSURE_CH, pressure);
    ThingSpeak.setField(VPD_CH, cal_vpd(temperature, humidity));
    ThingSpeak.setField(HI_CH, calculateHeatIndex(temperature, humidity));
    ThingSpeak.setField(RAIN_CH, rain_sensor.getRainfall(1));

    if (millis() - previous_upload_time > MINIMUM_UPLOAD_INTERVAL)// Check if minimum upload interval has elapsed
    {
    
      previous_upload_time = millis();// Update last upload time
      response = ThingSpeak.writeFields(TS_CH, WRITE_TS_API_KEY);

    if (response == 200) {
      Serial.println("Sensor data sent to ThingSpeak successfully!");
    } else {
      Serial.print("Error sending sensor data to ThingSpeak. HTTP error code: ");
      Serial.println(response);
    }

    sensors_flag = false;
    }

  }

  // Check if wind data needs to be uploaded
  if (wind_dir_flag && wind_speed_flag) {
    ThingSpeak.setField(WIND_SPD_CH, wind_speed);
    ThingSpeak.setField(WIND_DIR_CH, wind_direction);

    if (millis() - previous_upload_time > MINIMUM_UPLOAD_INTERVAL)// Check if minimum upload interval has elapsed
    {
      
      previous_upload_time = millis();
      response = ThingSpeak.writeFields(TS_CH, WRITE_TS_API_KEY);

    if (response == 200) {
      Serial.println("Wind data sent to ThingSpeak successfully!");
    } else {
      Serial.print("Error sending wind data to ThingSpeak. HTTP error code: ");
      Serial.println(response);
    }

    wind_dir_flag = false;
    wind_speed_flag = false;

    }


  }

  return response;
}


bool checkForSensorsError()
{
  if (errorInfo.ads1115 || errorInfo.sht31 || errorInfo.bme280)
  {
    return true;
  }

  else
  {
    return false;
  }
}


/**
 * @brief flashes the red led if an error is detected
*/
void flashRedLed()
{
  redLedState = !redLedState;

  // Set the LED brightness based on the state
  if (redLedState) {
    analogWrite(RED_LED_PIN, redLedBrightness); // Full brightness (ON)
  } else {
    analogWrite(RED_LED_PIN, 0);   // Off
  }
}

// Function to calculate the heat index
float calculateHeatIndex(float temperatureCelsius, float humidity) {
    // Convert Celsius to Fahrenheit
    float temperatureFahrenheit = (temperatureCelsius * 9.0 / 5.0) + 32.0;
    
    // Check if the temperature is in the valid range
    if (temperatureFahrenheit < 80.0 || humidity < 40.0) {
        return temperatureCelsius; // Heat index is approximately the same as the actual temperature in Celsius
    }
    
    // Calculate the heat index in Fahrenheit using the formula
    float heatIndexFahrenheit = -42.379 + 2.04901523 * temperatureFahrenheit + 10.14333127 * humidity
                      - 0.22475541 * temperatureFahrenheit * humidity - 0.00683783 * temperatureFahrenheit * temperatureFahrenheit
                      - 0.05481717 * humidity * humidity + 0.00122874 * temperatureFahrenheit * temperatureFahrenheit * humidity
                      + 0.00085282 * temperatureFahrenheit * humidity * humidity - 0.00000199 * temperatureFahrenheit * temperatureFahrenheit * humidity * humidity;
    
    // Ensure the heat index is not lower than the temperature in Fahrenheit
    if (heatIndexFahrenheit < temperatureFahrenheit) {
        return temperatureCelsius; // Return the original Celsius temperature
    }
    
    // Convert the heat index back to Celsius
    float heatIndexCelsius = (heatIndexFahrenheit - 32.0) * 5.0 / 9.0;
    
    return heatIndexCelsius;
}



