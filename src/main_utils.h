#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "sht_utils.h"
#include "bme_utils.h"
#include "ads_utils.h"
#include "wind_utils.h"
#include "time_utils.h"
#include "main_utils.h"
#include "wifi_utils.h"
#include "ThingSpeak.h"
#include "rain_utils.h"
#include "secrets.h"
#include "server.h"



extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

extern int currentDay;


// Pin Definitions
#define SDA 21
#define SCL 22
#define ONBOARD_LED 15
#define BUILTIN_LED 2

extern boolean sensors_flag;//flag to indicate if sensor data needs to be uploaded  
extern boolean wind_speed_flag;//flag to indicate if wind speed data needs to be uploaded
extern boolean wind_dir_flag;//flag to indicate if wind direction data needs to be uploaded 


extern unsigned long last_wind_sample_time;
extern unsigned long last_sensors_reading_time;
extern unsigned long last_serial_print_time;
extern unsigned long last_wifi_status_time;
extern const unsigned long SERIAL_PRINT_INTERVAL;
extern const unsigned long WIFI_STATUS_INTERVAL;
extern const unsigned long NTP_UPDATE_INTERVAL;
extern unsigned long last_ntp_update_time;
extern int latest_http_response;

extern const unsigned int MINIMUM_UPLOAD_INTERVAL;
extern const uint32_t CONNECT_TIMEOUT_MS;
extern unsigned int previous_upload_time;

//global sensors variables
extern float temperature;
extern float humidity;
extern float pressure;
extern float wind_speed;
extern int wind_direction;
extern float dir_voltage_debug;
extern float spd_voltage_debug;

extern int error_debug;


//ticker definitions
extern Ticker ticker;
extern Ticker greenLedTicker;
extern Ticker redLedTicker;

//leds definitions
extern const int greenLedBrightness;
extern bool greenLedState;
extern const int redLedBrightness;
extern bool redLedState ;


/**
 * @brief Error information structure
 */
struct ErrorInfo {
    bool ads1115; ///< ADS1115 error flag
    bool sht31;   ///< SHT31 error flag
    bool bme280;  ///< BME280 error flag
};

extern ErrorInfo errorInfo;

extern const int LED_PIN;
extern const int GREEN_LED_PIN;
extern const int RED_LED_PIN;
extern const unsigned long BLINK_INTERVAL;
extern unsigned long previousBlinkMillis;


/**
 * @brief Read sensor data
 *
 * @param[out] temperature - Temperature reading
 * @param[out] humidity - Humidity reading
 * @param[out] pressure - Pressure reading
 * @param[out] wind_speed - Wind speed reading
 * @param[out] wind_direction - Wind direction reading
 */
void readSensorData(float& temperature, float& humidity, float& pressure);
void readWindData(int& wind_direction, float& wind_speed);
/**
 * @brief Print sensor data
 *
 * @param[in] temperature - Temperature reading
 * @param[in] humidity - Humidity reading
 * @param[in] pressure - Pressure reading
 * @param[in] wind_speed - Wind speed reading
 * @param[in] wind_direction - Wind direction reading
 */
void printSensorData(float temperature, float humidity, float pressure, float wind_speed, int& wind_direction);

/**
 * @brief Initialize sensors
 */
void initSensors();

/**
 * @brief Set I2C communication
 */
void setI2C();

/**
 * @brief Print error information
 *
 * @param[in] errorInfo - Error information structure
 */
void printError(struct ErrorInfo& errorInfo);

/**
 * @brief Calculate vapor pressure deficit (VPD)
 *
 * @param t - Temperature in degrees Celsius
 * @param h - Humidity in percentage
 * @return The vapor pressure deficit
 */
float cal_vpd(float t, float h);


/**
 * @brief Reconnect to the WiFi network
 */

int uploadData();



void flashRedLed();


bool checkForSensorsError();

float calculateHeatIndex(float temperature, float humidity);

#endif // MAIN_UTILS_H
