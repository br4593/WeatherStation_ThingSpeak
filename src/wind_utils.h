#ifndef WIND_UTILS_H
#define WIND_UTILS_H

#include "main_utils.h"

// Constants for wind speed calculation
#define VOLTAGE_REF 5.0
#define SPEED_MAX 30

// Number of wind speed samples to store
extern const int ONE_MINUTE_IN_SECONDS;
extern int NUM_OF_SAMPLES_PER_MINUTE;
extern int WIND_AVERAGE_PERIOD_IN_MINUTES;
extern int NUM_OF_WIND_SAMPLES_PER_PERIOD;

// Time interval between wind speed samples in milliseconds
extern unsigned long WIND_SAMPLE_INTERVAL;



// Structure to represent wind direction
struct WindDirection {
  int number;
  String name;
  float voltageMin;
  float voltageMax;
};

// Function prototypes
void initWindSpeedSampling();
void stopWindSpeedSampling();
void initWindDirectionSampling();
void stopWindDirectionSampling();
float sampleWindSpeed(float* arr);
int sampleWindDirection(float* arr);
void readAverageWinds( float& wind_speed, int& wind_direction);
int readWindDir();
float readWindSpeed();
float computeSpeed(float voltage);
float mpsToKmph(float speed);
void resetArrayToZero(float* arr, int size);
String getWindDirectionString(int directionNumber);

#endif