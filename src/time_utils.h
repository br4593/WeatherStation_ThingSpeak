#ifndef TIME_UTILS_H
#define TIME_UTILS_H

<<<<<<< HEAD
#include <NTPClient.h>
=======
#include <time.h>
>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98
#include "main_utils.h"
extern const char* ntpServer;
extern const long gmtOffset_sec;

void printLocalTime();
void setupTime();
int calculateNthWeekdayOfMonth(int year, int month, int week, int weekday);

#endif  // TIME_UTILS_H
