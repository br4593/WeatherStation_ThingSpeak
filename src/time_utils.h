#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <time.h>
#include "main_utils.h"
extern const char* ntpServer;
extern const long gmtOffset_sec;

void printLocalTime();
void setupTime();
int calculateNthWeekdayOfMonth(int year, int month, int week, int weekday);

#endif  // TIME_UTILS_H
