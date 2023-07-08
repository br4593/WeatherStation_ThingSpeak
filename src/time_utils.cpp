#include "time_utils.h"

#include <WiFi.h>
#include <time.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 2 * 3600;            // GMT offset for Israel (2 hours ahead of UTC)

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    // Failed to obtain time
    Serial.println("Failed to obtain time");
    return;
  }

  // Adjust GMT offset and DST if necessary
  time_t adjustedTime = mktime(&timeinfo) + gmtOffset_sec;
  if (timeinfo.tm_isdst) {
    adjustedTime -= 3600;  // Subtract an hour during DST
  }

  // Convert adjusted time to a struct tm
  struct tm *adjustedTimeinfo = gmtime(&adjustedTime);

  // Format the time string
  char timeString[50];
  strftime(timeString, sizeof(timeString), "%A, %B %d %Y %H:%M:%S", adjustedTimeinfo);

  // Print the local time
  Serial.println(timeString);
}


void setupTime()
{
  // Initialize and get the time
  configTime(gmtOffset_sec, 0, ntpServer);  // Disable DST initially
  printLocalTime();
  
  // Enable or disable DST based on current date
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int currentYear = timeinfo.tm_year + 1900;
    int dstStartMonth = 3;   // DST starts in March (month index: 0-11)
    int dstEndMonth = 10;    // DST ends in October (month index: 0-11)
    int dstStartWeek = 5;    // DST starts on the last Sunday of March (week index: 1-5)
    int dstEndWeek = 5;      // DST ends on the last Sunday of October (week index: 1-5)

    // Calculate DST start and end dates for the current year
    int dstStartDay = calculateNthWeekdayOfMonth(currentYear, dstStartMonth, dstStartWeek, 0);  // 0: Sunday
    int dstEndDay = calculateNthWeekdayOfMonth(currentYear, dstEndMonth, dstEndWeek, 0);      // 0: Sunday
    
    // Determine if the current date falls within DST period
    bool isDst = false;
    if ((timeinfo.tm_mon > dstStartMonth || (timeinfo.tm_mon == dstStartMonth && timeinfo.tm_mday >= dstStartDay)) &&
        (timeinfo.tm_mon < dstEndMonth || (timeinfo.tm_mon == dstEndMonth && timeinfo.tm_mday <= dstEndDay))) {
      isDst = true;
    }
    
    // Adjust GMT offset and enable/disable DST accordingly
    if (isDst) {
      configTime(gmtOffset_sec + 3600, 3600, ntpServer);  // Enable DST
    } else {
      configTime(gmtOffset_sec, 0, ntpServer);            // Disable DST
    }
    
    // Print the updated local time and DST state
    printLocalTime();
    Serial.print("DST is ");
    Serial.println(isDst ? "enabled" : "disabled");
  } else {
    Serial.println("Failed to obtain time");
  }
}

int calculateNthWeekdayOfMonth(int year, int month, int week, int weekday)
{
  struct tm timeinfo = {0};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month;
  timeinfo.tm_mday = 1;
  
  mktime(&timeinfo);  // Normalize timeinfo structure
  
  int firstWeekday = timeinfo.tm_wday;
  int daysToAdd = (weekday - firstWeekday + 7) % 7 + (week - 1) * 7;
  
  timeinfo.tm_mday += daysToAdd;
  mktime(&timeinfo);  // Update timeinfo structure with the calculated date
  
  return timeinfo.tm_mday;
}
