#include "wind_utils.h"
#include <Arduino.h>

float aveWindSpdCalcArr[NUM_OF_WIND_SAMPLES];
float aveWindDirCalcArr[NUM_OF_WIND_SAMPLES];
int currentSpeedSampleIndex = 0;
int currentDirectionSampleIndex = 0;
unsigned long lastSpeedSampleTime = 0;
unsigned long lastDirectionSampleTime = 0;

WindDirection directions[] = {
  {0, "North Wind", 0.0, 0.7143},
  {45, "Northeasterly Wind", 0.7143, 1.4286},
  {90, "East Wind", 1.4286, 2.1429},
  {135, "Southeast Wind", 2.1429, 2.8571},
  {180, "South Wind", 2.8571, 3.5714},
  {215, "Southwest Wind", 3.5714, 4.2857},
  {270, "West Wind", 4.2857, 5.0},
  {315, "Northwest Wind", 0.0, 5.0}
};

/**
 * Initializes the wind speed sampling.
 * This function resets the wind speed calculation array and sets the initial sample index and time.
 */
void initWindSpeedSampling() {
  resetArrayToZero(aveWindSpdCalcArr, NUM_OF_WIND_SAMPLES);
  currentSpeedSampleIndex = 0;
  lastSpeedSampleTime = millis();
}

/**
 * Stops the wind speed sampling.
 * This function can be used for cleanup or additional actions when stopping wind speed sampling.
 */
void stopWindSpeedSampling() {
  // Perform any necessary cleanup or additional actions when stopping wind speed sampling
}

/**
 * Initializes the wind direction sampling.
 * This function resets the wind direction calculation array and sets the initial sample index and time.
 */
void initWindDirectionSampling() {
  resetArrayToZero(aveWindDirCalcArr, NUM_OF_WIND_SAMPLES);
  currentDirectionSampleIndex = 0;
  lastDirectionSampleTime = millis();
}

/**
 * Stops the wind direction sampling.
 * This function can be used for cleanup or additional actions when stopping wind direction sampling.
 */
void stopWindDirectionSampling() {
  // Perform any necessary cleanup or additional actions when stopping wind direction sampling
}

/**
 * Samples the wind speed.
 * This function calculates the average wind speed based on the provided voltage and a moving average calculation.
 *
 * @param voltage - The voltage reading from the wind speed sensor.
 * @param arr - The array to store the wind speed samples for the moving average calculation.
 * @return The average wind speed over the specified number of samples.
 */
float sampleWindSpeed(float voltage, float* arr) {
  float averageWindSpeed = -1;

  if ((millis() - lastSpeedSampleTime >= SAMPLE_INTERVAL)) {
    float windSpeed = mpsToKmph(computeSpeed(voltage));
    aveWindSpdCalcArr[currentSpeedSampleIndex] = windSpeed;

    Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
    Serial.println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
    Serial.print("Sample Index (Speed): ");
    Serial.println(currentSpeedSampleIndex);
    Serial.print("Current Wind Speed Sample: ");
    Serial.print(windSpeed);
    Serial.println(" km/h");
    Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");

    currentSpeedSampleIndex++;
    if (currentSpeedSampleIndex >= NUM_OF_WIND_SAMPLES) {
      currentSpeedSampleIndex = 0;

      float sumWindSpeeds = 0.0;
      for (int i = 0; i < NUM_OF_WIND_SAMPLES; i++) {
        sumWindSpeeds += aveWindSpdCalcArr[i];
      }
      averageWindSpeed = sumWindSpeeds / NUM_OF_WIND_SAMPLES;

      Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
      Serial.println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
      Serial.print("Average Wind Speed (last 10 minutes): ");
      Serial.print(averageWindSpeed);
      Serial.println(" km/h");
      Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");

      resetArrayToZero(arr, NUM_OF_WIND_SAMPLES);
      wind_speed_flag = true;
    }

    lastSpeedSampleTime = millis();
  }

  return averageWindSpeed;
}

/**
 * Samples the wind direction.
 * This function calculates the average wind direction based on the provided voltage and a moving average calculation.
 *
 * @param voltage - The voltage reading from the wind direction sensor.
 * @param arr - The array to store the wind direction samples for the moving average calculation.
 * @return The average wind direction over the specified number of samples.
 */
int sampleWindDirection(float voltage, float* arr) {
  int averageWindDirection = -1;

  if ((millis() - lastDirectionSampleTime >= SAMPLE_INTERVAL)) {
    for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++) {
      if (voltage >= directions[i].voltageMin && voltage < directions[i].voltageMax) {
        aveWindDirCalcArr[currentDirectionSampleIndex] = directions[i].number;

        Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
        Serial.println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
        Serial.print("Sample Index (Direction): ");
        Serial.println(currentDirectionSampleIndex);
        Serial.print("Current Wind Direction: ");
        Serial.println(directions[i].number);
        Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");

        break;
      }
    }

    currentDirectionSampleIndex++;
    if (currentDirectionSampleIndex >= NUM_OF_WIND_SAMPLES) {
      currentDirectionSampleIndex = 0;

      int sumWindDirection = 0;
      for (int i = 0; i < NUM_OF_WIND_SAMPLES; i++) {
        sumWindDirection += aveWindDirCalcArr[i];
      }

      averageWindDirection = sumWindDirection / NUM_OF_WIND_SAMPLES;
      Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
      Serial.println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
      Serial.print("Average Wind Direction (last 10 minutes): ");
      Serial.println(averageWindDirection);
      Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");

      resetArrayToZero(arr, NUM_OF_WIND_SAMPLES);
      wind_dir_flag = true;
    }

    lastDirectionSampleTime = millis();
  }

  return averageWindDirection;
}

/**
 * Returns the wind direction asa string based on the direction number.
 *
 * @param directionNumber - The wind direction number.
 * @return The wind direction as a string.
 */
String getWindDirectionString(int directionNumber) {
  for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++) {
    if (i == directionNumber) {
      return directions[i].name;
    }
  }
  return "Unknown";
}

/**
 * Computes the wind speed based on the voltage reading.
 *
 * @param voltage - The voltage reading from the wind speed sensor.
 * @return The wind speed in meters per second (m/s).
 */
float computeSpeed(float voltage) {
  return (voltage * SPEED_MAX) / VOLTAGE_REF;
}

/**
 * Converts the wind speed from meters per second (m/s) to kilometers per hour (km/h).
 *
 * @param speed - The wind speed in meters per second (m/s).
 * @return The wind speed in kilometers per hour (km/h).
 */
float mpsToKmph(float speed) {
  return speed * 3.6;
}

/**
 * Resets all elements of an array to zero.
 *
 * @param arr - The array to be reset.
 * @param size - The size of the array.
 */
void resetArrayToZero(float* arr, int size) {
  for (int i = 0; i < size; i++) {
    arr[i] = 0.0;
  }
}


