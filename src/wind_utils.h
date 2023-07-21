#ifndef WIND_UTILS_H
#define WIND_UTILS_H

#include "main_utils.h"

// Constants for wind speed calculation
#define VOLTAGE_REF 5.0
#define SPEED_MAX 30

// Number of wind speed samples to store
const int ONE_MINUTE_IN_SECONDS = 60;
const int NUM_OF_SAMPLES_PER_MINUTE = 3;
const int WIND_AVERAGE_PERIOD_IN_MINUTES = 15;
const int NUM_OF_WIND_SAMPLES_PER_PERIOD = NUM_OF_SAMPLES_PER_MINUTE * WIND_AVERAGE_PERIOD_IN_MINUTES;

// Time interval between wind speed samples in milliseconds
const unsigned long WIND_SAMPLE_INTERVAL = (ONE_MINUTE_IN_SECONDS / NUM_OF_SAMPLES_PER_MINUTE) * 1000;

// Array to store wind speed and direction samples
extern float aveWindSpdCalcArr[NUM_OF_WIND_SAMPLES_PER_PERIOD];
extern float aveWindDirCalcArr[NUM_OF_WIND_SAMPLES_PER_PERIOD];

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
