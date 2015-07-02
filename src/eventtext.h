#pragma once
#include <sys/time.h>
char* describe_current_events(GBitmap** hype,time_t* now);
char* describe_upcoming_events(time_t* now);
#define SHORT_TIME_LEN 7
void short_time(time_t* tim,char buffer[SHORT_TIME_LEN]);
