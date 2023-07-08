#ifndef WIND_UTILS_H
#define WIND_UTILS_H

#include "main_utils.h"

// Constants for wind speed calculation
#define VOLTAGE_REF 5.0
#define SPEED_MAX 30

// Number of wind speed samples to store
const int NUM_OF_WIND_SAMPLES = 10;

// Time interval between wind speed samples in milliseconds
const unsigned long SAMPLE_INTERVAL = 60000; // 1 minute interval in milliseconds

// Array to store wind speed samples
extern float aveWindSpdCalcArr[NUM_OF_WIND_SAMPLES];
extern float aveWindDirCalcArr[NUM_OF_WIND_SAMPLES];

// Structure to represent wind direction
struct WindDirection {
  int number;
  String name;
  float voltageMin;
  float voltageMax;
};

/**
 * Initializes the wind speed sampling.
 * This function sets up the necessary variables and arrays for wind speed sampling.
 */
void initWindSpeedSampling();

/**
 * Stops the wind speed sampling.
 * This function can be used for cleanup or additional actions when stopping wind speed sampling.
 */
void stopWindSpeedSampling();

/**
 * Initializes the wind direction sampling.
 * This function sets up the necessary variables and arrays for wind direction sampling.
 */
void initWindDirectionSampling();

/**
 * Stops the wind direction sampling.
 * This function can be used for cleanup or additional actions when stopping wind direction sampling.
 */
void stopWindDirectionSampling();

/**
 * Samples the wind speed and calculates the average over time.
 * This function takes a voltage reading from the wind speed sensor, stores it in the sample array,
 * and calculates the average wind speed over the specified number of samples.
 *
 * @param voltage - The voltage reading from the wind speed sensor.
 * @param arr - The array to store the wind speed samples for the moving average calculation.
 * @return The average wind speed over the specified number of samples.
 */
float sampleWindSpeed(float voltage, float* arr);

/**
 * Samples the wind direction and calculates the average over time.
 * This function takes a voltage reading from the wind direction sensor, stores it in the sample array,
 * and calculates the average wind direction over the specified number of samples.
 *
 * @param voltage - The voltage reading from the wind direction sensor.
 * @param arr - The array to store the wind direction samples for the moving average calculation.
 * @return The average wind direction over the specified number of samples.
 */
int sampleWindDirection(float voltage, float* arr);

/**
 * Computes the wind speed based on the voltage reading.
 *
 * @param voltage - The voltage reading from the wind speed sensor.
 * @return The wind speed in meters per second (m/s).
 */
float computeSpeed(float voltage);

/**
 * Converts the wind speed from meters per second (m/s) to kilometers per hour (km/h).
 *
 * @param speed - The wind speed in meters per second (m/s).
 * @return The wind speed in kilometers per hour (km/h).
 */
float mpsToKmph(float speed);

/**
 * Resets all elements of an array to zero.
 *
 * @param arr - The array to be reset.
 * @param size - The size of the array.
 */
void resetArrayToZero(float* arr, int size);

/**
 * Gets the wind direction string based on the direction number.
 *
 * @param directionNumber - The wind direction number.
 * @return The wind direction as a string.
 */
String getWindDirectionString(int directionNumber);

#endif
