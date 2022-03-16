#ifndef _RTC_LIB_H_
#define _RTC_LIB_H_
#include "main.h"
#include "rtc.h"
#define DAY_HOUR 24
#define HOUR_MINUTE 60
#define MINUTE_SECOND 60
void RTC_Time_Minute_Increament(RTC_TimeTypeDef *time);

void RTC_Time_Minute_Decreament(RTC_TimeTypeDef *time);

void RTC_Time_Hour_Increament(RTC_TimeTypeDef *time);

void RTC_Time_Hour_Decreament(RTC_TimeTypeDef *time);

void RTC_Time_ResetHMS(RTC_TimeTypeDef *time);

uint32_t RTC_Time_Minus2Seconds(RTC_TimeTypeDef *tim1, RTC_TimeTypeDef *tim2);

bool RTC_Time_Equal(RTC_TimeTypeDef *tim1, RTC_TimeTypeDef *tim2);
#endif // _RTC_LIB_H_