#ifndef WIND_UTILS_H
#define WIND_UTILS_H

#include "main_utils.h"

// Constants for wind speed calculation
#define VOLTAGE_REF 5.0
#define SPEED_MAX 30



// Time interval between wind speed samples in milliseconds
extern const unsigned long WIND_SAMPLE_INTERVAL;

extern const int WIND_SPEED_SENSOR_ADC_CHANNEL;
extern const int WIND_DIR_SENSOR_ADC_CHANNEL;


// Structure to represent wind direction
struct WindDirection {
  int degrees;
  String name;
  float voltageMin;
  float voltageMax;
};



float aveWindSpeed();
float aveWindDir();
void readAverageWinds( float& wind_speed, int& wind_direction);
int readWindDir();
float readWindSpeed();
float computeSpeed(float voltage);
float mpsToKmph(float speed);

void setupWindSensorChannels(int windSpeedADCCh, int windDirADCCh);

#endif
