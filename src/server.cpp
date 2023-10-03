#include "server.h"

void debugPage()
{
    server.on("/sensors", HTTP_GET, [](){
  String html = "<html><head>";
  html += "<meta http-equiv=\"refresh\" content=\"10\">";  // Auto-refresh every 10 seconds
  html += "</head><body>";
  html += "<h1>Sensor Data</h1>";
  html += "<p>Temperature: " + String(temperature, 2) + " &deg;C</p>";
  html += "<p>Humidity: " + String(humidity, 2) + " %</p>";
  html += "<p>Pressure: " + String(pressure, 2) + " hPa</p>";
  html += "<p>Wind Speed: " + String(wind_speed, 2) + " km/h</p>";
  html += "<p>Wind Speed Voltage: " + String(spd_voltage_debug, 2) + " V</p>";
  html += "<p>Wind Direction: " + String(wind_direction) + " degrees</p>";
  html += "<p>Wind Direction Voltage: " + String(dir_voltage_debug, 2) + " V</p>";
  html += "<p>ADS1115 error status: " + String(errorInfo.ads1115) + "</p>";
  html += "<p>SHT31 error status: " + String(errorInfo.sht31) + "</p>";
  html += "<p>BME280 error status: " + String(errorInfo.bme280) + "</p>";
  html += "<p>Error status: " + String(error_debug) + "</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
});

}