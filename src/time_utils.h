#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <NTPClient.h>
#include "main_utils.h"
#include <ESP32Time.h>

extern ESP32Time rtc;
extern const char* ntpServer;
extern const long gmtOffset_sec;
extern struct tm timeinfo;
extern const char* JERUSALEM_TZ;
extern int currentDay;

void printLocalTime();
boolean initTime(String timezone);
void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst);
void updateNtpTime();
void updateRtcTime();


#endif  // TIME_UTILS_H
