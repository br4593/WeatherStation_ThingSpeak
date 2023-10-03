#include "wind_utils.h"
#include <Arduino.h>

const int ONE_MINUTE_IN_SECONDS = 60;
/*int NUM_OF_SAMPLES_PER_MINUTE = 60;
int WIND_AVERAGE_PERIOD_IN_MINUTES = 1;
int NUM_OF_WIND_SAMPLES_PER_PERIOD = NUM_OF_SAMPLES_PER_MINUTE * WIND_AVERAGE_PERIOD_IN_MINUTES;
//unsigned long WIND_SAMPLE_INTERVAL = (ONE_MINUTE_IN_SECONDS / NUM_OF_SAMPLES_PER_MINUTE) * 1000;



const int MAX_NUM_OF_SAMPLES_PER_MINUTE = 10;
const int MAX_WIND_AVERAGE_PERIOD_IN_MINUTES = 60;
const int MAX_NUM_OF_WIND_SAMPLES_PER_PERIOD = MAX_NUM_OF_SAMPLES_PER_MINUTE * MAX_WIND_AVERAGE_PERIOD_IN_MINUTES;

float aveWindSpdCalcArr[MAX_NUM_OF_WIND_SAMPLES_PER_PERIOD];
float aveWindDirCalcArr[MAX_NUM_OF_WIND_SAMPLES_PER_PERIOD];
int currentSpeedSampleIndex = 0;
int currentDirectionSampleIndex = 0;
*/


unsigned long start_of_speed_sample_time = 0;
unsigned long last_speed_sample_time = 0;
unsigned long last_direction_sample_time = 0;
unsigned long start_of_direction_sample_time = 0;
unsigned long WIND_SAMPLE_INTERVAL = 1000;
unsigned long wind_average_period_length = 60000;
float sum_of_wind_speeds = 0.0;
int num_of_speed_samples = 0;
float sum_of_wind_directions = 0.0;
int num_of_direction_samples = 0;


int wind_speed_sensor_adc_channel = 0;
int wind_dir_sensor_adc_channel = 1;

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



float aveWindSpeed()
{
  if (millis() - last_speed_sample_time > WIND_SAMPLE_INTERVAL)
  {
    sum_of_wind_speeds += readWindSpeed();
    num_of_speed_samples++;
    last_speed_sample_time = millis();
  }

  if (millis() - start_of_speed_sample_time > wind_average_period_length)
  {
    start_of_speed_sample_time = millis();
    wind_speed_flag = true;
    // Calculate the average and reset variables
    float average_speed = num_of_speed_samples > 0 ? sum_of_wind_speeds / num_of_speed_samples : 0.0;
    sum_of_wind_speeds = 0.0;
    num_of_speed_samples = 0;
    return average_speed;
  }

  return wind_speed; //return default value
}


float aveWindDir()
{
  if (millis() - last_direction_sample_time > WIND_SAMPLE_INTERVAL)
  {
    sum_of_wind_directions += readWindDir();
    num_of_direction_samples++;
    last_direction_sample_time = millis();
  }

  if (millis() - start_of_direction_sample_time > wind_average_period_length)
  {
    start_of_direction_sample_time = millis();
    wind_dir_flag = true;
    // Calculate the average and reset variables
    float average_direction = num_of_direction_samples > 0 ? sum_of_wind_directions / num_of_direction_samples : 0.0;
    sum_of_wind_directions = 0.0;
    num_of_direction_samples = 0;
    return average_direction;
  }

  return wind_direction; //return default value
}



/*
float sampleWindSpeed(float* arr) {

  if ((millis() - last_speed_sample_time >= WIND_SAMPLE_INTERVAL)) {
    float temp_wind_speed = readWindSpeed();
    aveWindSpdCalcArr[currentSpeedSampleIndex] = temp_wind_speed;
    last_speed_sample_time = millis();


    // Debug prints for the current sample
    Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
    Serial.println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
    Serial.print("Sample Index (Speed): ");
    Serial.println(currentSpeedSampleIndex + 1);
    Serial.print(" Out of: ");
    Serial.println(NUM_OF_WIND_SAMPLES_PER_PERIOD);
    Serial.print("Current Wind Speed Sample: ");
    Serial.print(temp_wind_speed);
    Serial.println(" km/h");
    Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");

    currentSpeedSampleIndex++;
    if (currentSpeedSampleIndex >= NUM_OF_WIND_SAMPLES_PER_PERIOD) {
      currentSpeedSampleIndex = 0;

      float sumWindSpeeds = 0.0;
      for (int i = 0; i < NUM_OF_WIND_SAMPLES_PER_PERIOD; i++) {
        sumWindSpeeds += aveWindSpdCalcArr[i];
      }
      float averageWindSpeed = sumWindSpeeds / NUM_OF_WIND_SAMPLES_PER_PERIOD;

      // Debug prints for the average wind speed over the specified period
      Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
      Serial.println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
      Serial.print("Average Wind Speed (last ");
      Serial.print(WIND_AVERAGE_PERIOD_IN_MINUTES);
      Serial.print(" minutes): ");
      Serial.print(averageWindSpeed);
      Serial.println(" km/h");
      Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");

      resetArrayToZero(arr, NUM_OF_WIND_SAMPLES_PER_PERIOD);
      wind_speed_flag = true;
      return averageWindSpeed;
    }
  }

  // If there's no new sample yet, return the default value (-1)
  return wind_speed;
}*/

/*
int sampleWindDirection(float* arr) {

  unsigned long lastDirectionSampleTime = 0;
    float temp_wind_dir = readWindDir();
    aveWindDirCalcArr[currentDirectionSampleIndex] = temp_wind_dir;
        lastDirectionSampleTime = millis();

    // Debug prints for the current sample
    Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
    Serial.println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
    Serial.print("Sample Index (Direction): ");
    Serial.println(currentDirectionSampleIndex + 1);
    Serial.print(" Out of: ");
    Serial.println(NUM_OF_WIND_SAMPLES_PER_PERIOD);
    Serial.print("Current Wind Direction: ");
    Serial.println(temp_wind_dir);
    Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
    currentDirectionSampleIndex++; // Move this line here to update index only after the sample is taken
  }

  if (currentDirectionSampleIndex >= NUM_OF_WIND_SAMPLES_PER_PERIOD) {
    currentDirectionSampleIndex = 0;

    int sumWindDirection = 0;
    for (int i = 0; i < NUM_OF_WIND_SAMPLES_PER_PERIOD; i++) {
      sumWindDirection += aveWindDirCalcArr[i];
    }

    int averageWindDirection = sumWindDirection / NUM_OF_WIND_SAMPLES_PER_PERIOD;

    // Debug prints for the average wind direction over the specified period
    Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
    Serial.println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
    Serial.print("Average Wind Direction (last ");
    Serial.print(WIND_AVERAGE_PERIOD_IN_MINUTES);
    Serial.print(" minutes): ");
    Serial.println(averageWindDirection);
    Serial.println("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");

    resetArrayToZero(aveWindDirCalcArr, NUM_OF_WIND_SAMPLES_PER_PERIOD); // Reset the array, not 'arr'
    wind_dir_flag = true;

    return averageWindDirection;
  }

  return wind_direction;
}*/



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

float readWindSpeed()
{
  // Read wind speed from ADS1115 analog-to-digital converter
  int16_t wind_speed_adc_ch_reading = ads.readADC_SingleEnded(wind_speed_sensor_adc_channel);
  float wind_speed_volts = ads.computeVolts(wind_speed_adc_ch_reading);
  float temp_wind_speed = mpsToKmph(computeSpeed(wind_speed_volts));

  return temp_wind_speed;
}

void readAverageWinds(float& wind_speed, int& wind_direction)
{
  float temp_ave_wind_speed = aveWindSpeed();
  int temp_ave_wind_dir = aveWindDir();

  if (wind_speed_flag) {
    wind_speed = temp_ave_wind_speed;
  }

  if (wind_dir_flag) {
    wind_direction = temp_ave_wind_dir;
  }
}

int readWindDir()
{
  Serial.println("Debugging readWindDir Function");

  // Read ADC channel for wind direction sensor
  int16_t wind_dir_adc_ch_reading = ads.readADC_SingleEnded(wind_dir_sensor_adc_channel);

  // Convert ADC reading to voltage
  float wind_dir_volts = ads.computeVolts(wind_dir_adc_ch_reading);

  Serial.print("Current volts is: ");
  Serial.println(wind_dir_volts);

  // Variable to store the wind direction
  int temp_wind_dir = -1;

  // Find the corresponding wind direction based on voltage
  for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++) {
    if (wind_dir_volts >= directions[i].voltageMin && wind_dir_volts < directions[i].voltageMax) {
      Serial.print("Found a direction: ");
      Serial.println(directions[i].name);
      temp_wind_dir = directions[i].number;
      break; // Exit the loop once a direction is found
    }
  }

  // Handle the case when no direction is found
  if (temp_wind_dir == -1) {
    Serial.println("Unknown wind direction.");
    // You can either return a default direction or an error code here
    // Example: return 99; or return an enum representing "unknown direction"
  }

  return temp_wind_dir;
}