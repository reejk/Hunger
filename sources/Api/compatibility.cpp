#include "compatibility.h"

#ifdef WIN32
#include <time.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <cmath>
#include <limits>

Timestamp getCurrentTime() {
#ifdef WIN32
	SYSTEMTIME st;
	GetSystemTime(&st);
	return (time(NULL) * 1000 + st.wMilliseconds);
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec/1000);
#endif
}

void getTime(Time& currentTime) {
#ifdef WIN32
	SYSTEMTIME sm;
	GetLocalTime(&sm);
	currentTime.year = sm.wYear;
	currentTime.month = sm.wMonth;
	currentTime.dayOfWeek = sm.wDayOfWeek;
	currentTime.day = sm.wDay;
	currentTime.hour = sm.wHour;
	currentTime.minute = sm.wMinute;
	currentTime.second = sm.wSecond;
	currentTime.milliseconds = sm.wMilliseconds;
#else
	// TODO: Fix it
	time_t timet;
	time(timet);
	tm local = localtime(timet);
	currentTime.year = local.tm_year;
	currentTime.month = local.tm_mon;
	currentTime.dayOfWeek = local.tm_wday;
	currentTime.day = local.tm_mday;
	currentTime.hour = local.tm_hout;
	currentTime.minute = local.tm_min;
	currentTime.second = local.tm_sec;
	currentTime.milliseconds = getCurrentTime() % 1000;
#endif
}

bool equal(float a, float b) {
	return std::fabs(a - b) < 2*std::numeric_limits<float>::epsilon();
}
