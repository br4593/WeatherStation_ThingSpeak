#include "time_utils.h"
#include <time.h>

ESP32Time rtc;
const char* ntpServer = "pool.ntp.org";
struct tm timeinfo;
const char* JERUSALEM_TZ = "IST-2IDT,M3.4.4/26,M10.5.0";
int currentDay = 0;

//const long  gmtOffset_sec = 2 * 3600;            // GMT offset for Israel (2 hours ahead of UTC)

void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

boolean initTime(String timezone){
  //struct tm timeinfo;

  Serial.println("Setting up time");
  configTime(0, 0, ntpServer);    // First connect to NTP server, with 0 TZ offset
  if(!getLocalTime(&timeinfo)){
    Serial.println("  Failed to obtain time");
    return false;
  }
  Serial.println("  Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
  last_ntp_update_time = millis();
  return true;
}

void printLocalTime(){
  //struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}


void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst){
  //struct tm tm;

  timeinfo.tm_year = yr - 1900;   // Set date
  timeinfo.tm_mon = month-1;
  timeinfo.tm_mday = mday;
  timeinfo.tm_hour = hr;      // Set time
  timeinfo.tm_min = minute;
  timeinfo.tm_sec = sec;
  timeinfo.tm_isdst = isDst;  // 1 or 0
  time_t t = mktime(&timeinfo);
  Serial.printf("Setting time: %s", asctime(&timeinfo));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}


void updateNtpTime(){
  if(millis() - last_ntp_update_time > NTP_UPDATE_INTERVAL){
    if(initTime(JERUSALEM_TZ)){
      Serial.println("  Updated time from NTP");
    }

    else{
      Serial.println("  Failed to update time from NTP");
    }
  }
}

void updateRtcTime(){
  if (currentDay != timeinfo.tm_mday){
    rtc.setTimeStruct(timeinfo);
    currentDay = timeinfo.tm_mday;
  }
}