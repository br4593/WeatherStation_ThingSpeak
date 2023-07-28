#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include "sht_utils.h"
#include "bme_utils.h"
#include "ads_utils.h"
#include "wind_utils.h"
#include "time_utils.h"
#include <WiFi.h>
#include "ThingSpeak.h"
//#include <ArduinoOTA.h>

#define WIND_SPEED_SENSOR_ADC_CH 1
#define WIND_DIR_SENSOR_ADC_CH 3
#define SENSOR_READING_INTERVAL (15 * 60 * 1000) // 15 minutes in milliseconds
#define WIND_READING_INTERVAL (60 * 1000) // 1 minute in milliseconds

#define NETWORK "rodoy"
#define PASSWORD "0544543538"

//ThingSpeak definitions
#define TS_CH 2209504
#define TS_API_KEY "OF6CVEMHFWTZXMYO"
#define HUMIDITY_CH 2
#define TEMP_CH 1
#define PRESSURE_CH 3
#define WIND_SPD_CH 4
#define WIND_DIR_CH 5
#define VPD_CH 6

// Pin Definitions
#define SDA 33
#define SCL 35
#define ONBOARD_LED 15

extern boolean wifi_flag;
extern boolean sensors_flag;
extern boolean wind_speed_flag;
extern boolean wind_dir_flag;
extern unsigned long last_wind_sample_time;
extern unsigned long last_sensors_reading_time;
extern bool ads1115_error;
extern bool sht31_error;
extern bool bme280_error;
extern unsigned long last_serial_print_time;
extern unsigned long last_wifi_status_time;
extern const unsigned long serial_print_interval;
extern const unsigned long wifi_status_interval;

extern WiFiClient client;

extern float temperature;
extern float humidity;
extern float pressure;
extern float wind_speed;
extern int wind_direction;

extern unsigned long myChannelNumber;
extern const char* myWriteAPIKey;

/**
 * @brief Error information structure
 */
struct ErrorInfo {
  bool ads1115; ///< ADS1115 error flag
  bool sht31;   ///< SHT31 error flag
  bool bme280;  ///< BME280 error flag
};

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
 * @brief Reset Arduino Cloud variables
 */
void resetArduinoCloudVariables();

/**
 * @brief Set I2C communication
 */
void setI2C();

/**
 * @brief Print error information
 *
 * @param[in] errorInfo - Error information structure
 */
void printError(const ErrorInfo& errorInfo);

/**
 * @brief Calculate vapor pressure deficit (VPD)
 *
 * @param t - Temperature in degrees Celsius
 * @param h - Humidity in percentage
 * @return The vapor pressure deficit
 */
float cal_vpd(float t, float h);

/**
 * @brief Connect to the WiFi network
 */
void connectToWiFi();

/**
 * @brief Reconnect to the WiFi network
 */
void reconnectToWiFi();

/**
 * @brief Upload sensor data to ThingSpeak
 */
void uploadData();

/**
 * @brief Disconnect from the WiFi network
 */
void disconnectFromWiFi();

/**
 * @brief Print WiFi status
 */
void printWiFiStatus();

void resetFlags();

#endif // MAIN_UTILS_H
