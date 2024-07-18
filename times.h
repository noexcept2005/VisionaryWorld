//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once

DWORD CJZAPI ToMilliSecond(clock_t d)	
{
	return DWORD(d / (double)CLOCKS_PER_SEC * 1000.0);
}
int CJZAPI GetYear(void) {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_year + 1900);
}
int CJZAPI GetMonth(void) {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_mon + 1);
}
int CJZAPI GetDay(void) {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_mday);
}
int CJZAPI GetHour(void) {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_hour);
}
int CJZAPI GetMinute(void) {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_min);
}
int CJZAPI GetSecond(void) {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_sec);
}