#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "sht_utils.h"
#include "bme_utils.h"
#include "ads_utils.h"
#include "wind_utils.h"
#include "time_utils.h"
#include "main_utils.h"
#include "wifi_utils.h"
#include "ThingSpeak.h"


//ThingSpeak definitions
#define TS_CH 2209504
#define TS_API_KEY "OF6CVEMHFWTZXMYO"
#define HUMIDITY_CH 2
#define TEMP_CH 1
#define PRESSURE_CH 3
#define WIND_SPD_CH 4
#define WIND_DIR_CH 5
#define VPD_CH 6
extern unsigned long myChannelNumber;
extern const char* myWriteAPIKey;


// Pin Definitions
#define SDA 33
#define SCL 35
#define ONBOARD_LED 15


extern boolean sensors_flag;//flag to indicate if sensor data needs to be uploaded  
extern boolean wind_speed_flag;//flag to indicate if wind speed data needs to be uploaded
extern boolean wind_dir_flag;//flag to indicate if wind direction data needs to be uploaded 


extern unsigned long last_wind_sample_time;
extern unsigned long last_sensors_reading_time;
extern unsigned long last_serial_print_time;
extern unsigned long last_wifi_status_time;
extern const unsigned long serial_print_interval;
extern const unsigned long wifi_status_interval;

extern const unsigned int MINIMUM_UPLOAD_INTERVAL;
extern const uint32_t connectTimeoutMs;

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

void uploadData();



void flashRedLed();


bool checkForSensorsError();

#endif // MAIN_UTILS_H
