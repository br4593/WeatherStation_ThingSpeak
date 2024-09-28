#ifndef RAIN_UTILS_H
#define RAIN_UTILS_H

#define GRAVITY_RELAY_PIN 4

#include "DFRobot_RainfallSensor.h"
#include "main_utils.h"

extern DFRobot_RainfallSensor_I2C rain_sensor;

extern float currentRainfall;


void setupRainFallSensor();

#endif // RAIN_UTILS_H
