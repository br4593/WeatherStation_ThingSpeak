#include "wind_utils.h"



unsigned long start_of_speed_sample_time = 0;
unsigned long last_speed_sample_time = 0;
unsigned long last_direction_sample_time = 0;
unsigned long start_of_direction_sample_time = 0;

unsigned long wind_average_period_length = 60000;
float sum_of_wind_speeds = 0.0;
int num_of_speed_samples = 0;
float sum_of_wind_directions = 0.0;
int num_of_direction_samples = 0;

const unsigned long WIND_SAMPLE_INTERVAL = 1000;
const int WIND_SPEED_SENSOR_ADC_CHANNEL = 1;
const int WIND_DIR_SENSOR_ADC_CHANNEL = 0;
const int MPS_TO_KMPH_FACTOR = 3.6;

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
  return speed * MPS_TO_KMPH_FACTOR;
}


float readWindSpeed()
{
  // Read wind speed from ADS1115 analog-to-digital converter
  int16_t wind_speed_adc_ch_reading = ads.readADC_SingleEnded(WIND_SPEED_SENSOR_ADC_CHANNEL);
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
  int16_t wind_dir_adc_ch_reading = ads.readADC_SingleEnded(WIND_DIR_SENSOR_ADC_CHANNEL);

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
      temp_wind_dir = directions[i].degrees;
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