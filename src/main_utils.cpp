#include "main_utils.h"

// Error flags for sensors
bool ads1115_error = false;
bool sht31_error = false;
bool bme280_error = false;


// Variables to track the last reading time
unsigned long last_serial_print_time = 0;
unsigned long last_wifi_status_time = 0;
unsigned long last_sensors_reading_time = 0;
unsigned long last_wind_sample_time = 0;
const unsigned long serial_print_interval = 60000; // Print sensor data every 1 minute
const unsigned long wifi_status_interval = 30000; 

int SENSORS_READING_INTERVAL_IN_MINUTES = 15;
int SENSORS_READING_INTERVAL = SENSORS_READING_INTERVAL_IN_MINUTES * ONE_MINUTE_IN_SECONDS * 1000;

const unsigned int MINIMUM_UPLOAD_INTERVAL = 15000;
unsigned int previous_upload_time = 0;

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


WiFiNetwork* networks = nullptr;
int numNetworks = 0;

void addNetwork(const char* ssid, const char* password) {
    // Allocate memory for the new network
    WiFiNetwork* newNetworks = (WiFiNetwork*)realloc(networks, (numNetworks + 1) * sizeof(WiFiNetwork));
    if (newNetworks) {
        networks = newNetworks;
        networks[numNetworks] = {ssid, password};
        numNetworks++;
    } else {
        Serial.println("Memory allocation failed for adding a new network.");
    }
}

const uint32_t connectTimeoutMs = 10000;


const int LED_PIN = 15; // Replace with the appropriate onboard LED pin for your ESP32 board
const unsigned long BLINK_INTERVAL = 500; // LED blink interval in milliseconds
unsigned long previousBlinkMillis = 0;

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
    last_sensors_reading_time = millis();
    sensors_flag = true;
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

/*void addMultiWifi(WiFiNetwork networks[], int numOfNetworks)
{
  int i = 0;

  for (i=0;i<numOfNetworks;i++)
  {
    wifiMulti.addAP(networks[i].ssid,networks[i].password);
  }
}*/


/*void connectToWifi()
{
  WiFi.mode(WIFI_STA);
    Serial.println("Connecting Wifi...");
  if(wifiMulti.run() == WL_CONNECTED) {
printWiFiStatus();
wifi_flag = true;
  }
}*/








/**
 * Uploads sensor data to ThingSpeak.
 */
void uploadData() {
  // Check if sensor data needs to be uploaded
  if (sensors_flag) {
    ThingSpeak.setField(HUMIDITY_CH, humidity);
    ThingSpeak.setField(TEMP_CH, temperature);
    ThingSpeak.setField(PRESSURE_CH, pressure);
    ThingSpeak.setField(VPD_CH, cal_vpd(temperature, humidity));

    if (millis() - previous_upload_time > MINIMUM_UPLOAD_INTERVAL)
    {
      previous_upload_time = millis();
      int response = ThingSpeak.writeFields(TS_CH, TS_API_KEY);

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

    if (millis() - previous_upload_time > MINIMUM_UPLOAD_INTERVAL)
    {
      previous_upload_time = millis();
      int response = ThingSpeak.writeFields(TS_CH, TS_API_KEY);

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
}



void resetFlags()
{
  wind_dir_flag = false;
  wind_speed_flag = false;
  sensors_flag = false;
}






void startWebServer()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        // Prepare the HTML response
        String html = "<html><body>";
        html += "<h1>Rodoy Weather Station</h1>";
        html += "<p>Connected to Wi-Fi: " + WiFi.SSID() + "</p>";

        // Get and include local time in the HTML response
        struct tm timeinfo;
        if (getLocalTime(&timeinfo))
        {
            char timeString[50];
            strftime(timeString, sizeof(timeString), "Local Time: %A, %B %d %Y %H:%M:%S", &timeinfo);
            html += "<p>" + String(timeString) + "</p>";
        }
        else
        {
            html += "<p>Failed to obtain local time</p>";
        }

        // Include sensor readings in the HTML response
        html += "<h2>Sensor Readings</h2>";
        html += "<p>Temperature: " + String(temperature) + " &#8451;</p>";
        html += "<p>Humidity: " + String(humidity) + " %</p>";
        html += "<p>Pressure: " + String(pressure) + " hPa</p>";
        html += "<p>Wind Speed: " + String(wind_speed) + " m/s</p>";
        html += "<p>Wind Direction: " + String(wind_direction) + " &#176;</p>";

        // Add the option menu to change wind samples number, average period, and sensors update interval
        html += "<h2>Wind Settings</h2>";
        html += "<form method='POST' action='/settings'>";
        html += "<label for='num_of_samples'>Number of Wind Samples per Minute:</label>";
        html += "<input type='number' name='num_of_samples' value='" + String(NUM_OF_SAMPLES_PER_MINUTE) + "'><br>";
        html += "<label for='average_period'>Wind Average Period in Minutes:</label>";
        html += "<input type='number' name='average_period' value='" + String(WIND_AVERAGE_PERIOD_IN_MINUTES) + "'><br>";
        html += "<h2>Sensors Update Interval</h2>";
        html += "<label for='sensors_update_interval'>Sensors Update Interval in Minutes:</label>";
        html += "<input type='number' name='sensors_update_interval' value='" + String(SENSORS_READING_INTERVAL_IN_MINUTES) + "'><br>";
        html += "<input type='submit' value='Save Settings'>";
        html += "</form>";

        // Add JavaScript script to display a popup message
        html += "<script>";
        html += "function showPopup() {";
        html += "  alert('Settings updated successfully!');";
        html += "}";
        html += "</script>";

        // Check if the URL query parameter "updated" is set to "true" (indicating settings were updated)
        if (request->hasParam("updated")) {
            String updatedParamValue = request->getParam("updated")->value();
            if (updatedParamValue.equals("true")) {
                // Call the showPopup function after the page is loaded
                html += "<script>";
                html += "window.onload = showPopup;";
                html += "</script>";
            }
        }

        html += "</body></html>";

        // Send the response to the client
        request->send(200, "text/html", html);
    });

 // Handle the form submission and update wind settings and sensors update interval
server.on("/settings", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasArg("num_of_samples") && request->hasArg("average_period") && request->hasArg("sensors_update_interval"))
    {
        NUM_OF_SAMPLES_PER_MINUTE = request->arg("num_of_samples").toInt();
        WIND_AVERAGE_PERIOD_IN_MINUTES = request->arg("average_period").toInt();
        SENSORS_READING_INTERVAL_IN_MINUTES = request->arg("sensors_update_interval").toInt();

        // Recalculate WIND_SAMPLE_INTERVAL and NUM_OF_WIND_SAMPLES_PER_PERIOD based on the updated settings
        WIND_SAMPLE_INTERVAL = (ONE_MINUTE_IN_SECONDS / NUM_OF_SAMPLES_PER_MINUTE) * 1000;
        NUM_OF_WIND_SAMPLES_PER_PERIOD = NUM_OF_SAMPLES_PER_MINUTE * WIND_AVERAGE_PERIOD_IN_MINUTES;
        SENSORS_READING_INTERVAL = SENSORS_READING_INTERVAL_IN_MINUTES * ONE_MINUTE_IN_SECONDS * 1000;

        // Print the updated settings
        Serial.println("Number of Wind Samples per Minute: " + String(NUM_OF_SAMPLES_PER_MINUTE));
        Serial.println("Wind Average Period in Minutes: " + String(WIND_AVERAGE_PERIOD_IN_MINUTES));
        Serial.println("Sensors Update Interval in Minutes: " + String(SENSORS_READING_INTERVAL_IN_MINUTES));

        // Redirect back to the main page after updating the settings
        String redirectUrl = "/?updated=true";
        // Send a response to display a button and redirect back to the home page
        String redirectHtml = "<html><body>";
        redirectHtml += "<h1>Settings updated. Redirecting...</h1>";
        // Add a button to go back home
        redirectHtml += "<button onclick='window.location.href=\"/\"'>Go Back Home</button>";
        redirectHtml += "<script>setTimeout(function(){window.location.href=\"/\";}, 5000);</script>";
        redirectHtml += "</body></html>";
        request->send(200, "text/html", redirectHtml);
        // Redirect after 5 seconds, in case the button is not clicked
        request->redirect(redirectUrl);
    }
});

    // New route to display the list of SSIDs
    server.on("/list_ssids", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<html><body>";
        html += "<h1>List of Available SSIDs</h1>";
        html += "<ul>";

        for (size_t i = 0; i < numNetworks; i++) {
            html += "<li>" + String(networks[i].ssid) + "</li>";
        }

        html += "</ul>";
        html += "</body></html>";

        request->send(200, "text/html", html);
    });

    // New route to add a new network
    server.on("/add_network", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasArg("ssid") && request->hasArg("password")) {
            String ssid = request->arg("ssid");
            String password = request->arg("password");
            addNetwork(ssid.c_str(), password.c_str());
            Serial.println("New network added: " + ssid + ", " + password);
        }

        // Redirect back to the main page after adding the network
        request->send(303);
        request->redirect("/");
    });

    // ... (existing code for the main page)

    // Start the web server
    server.begin();
}

