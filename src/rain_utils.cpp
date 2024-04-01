#include "rain_utils.h"

DFRobot_RainfallSensor_I2C rain_sensor(&Wire);

float currentRainfall = 0.0;

void setupRainFallSensor() {
  rain_sensor.begin();
    Serial.print("vid:\t");
  Serial.println(rain_sensor.vid,HEX);
  Serial.print("pid:\t");
  Serial.println(rain_sensor.pid,HEX);
  Serial.print("Version:\t");
  Serial.println(rain_sensor.getFirmwareVersion());
}

float resetRainfall() {
  currentRainfall = 0.0;
  return currentRainfall;
}