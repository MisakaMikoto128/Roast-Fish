#include "RTCLib.h"
void RTC_Time_Minute_Increament(RTC_TimeTypeDef *time)
{
  if (time->Minutes == 59)
  {
    time->Minutes = 0;
  }
  else
  {
    time->Minutes++;
  }
}

void RTC_Time_Minute_Decreament(RTC_TimeTypeDef *time)
{
  if (time->Minutes == 0)
  {
    time->Minutes = 59;
  }
  else
  {
    time->Minutes--;
  }
}

void RTC_Time_Hour_Increament(RTC_TimeTypeDef *time)
{
  if (time->Hours == 23)
  {
    time->Hours = 0;
  }
  else
  {
    time->Hours++;
  }
}

void RTC_Time_Hour_Decreament(RTC_TimeTypeDef *time)
{
  if (time->Hours == 0)
  {
    time->Hours = 23;
  }
  else
  {
    time->Hours--;
  }
}

void RTC_Time_ResetHMS(RTC_TimeTypeDef *time)
{
  time->Hours = 0;
  time->Minutes = 0;
  time->Seconds = 0;
}

/**
 * @brief 24H-format time minus 2 seconds,tim1 occured after tim2
 * 
 * @param tim1 
 * @param tim2 
 * @return uint32_t 
 */
uint32_t RTC_Time_Minus2Seconds(RTC_TimeTypeDef *tim1, RTC_TimeTypeDef *tim2){
    uint32_t hour = tim1->Hours - tim2->Hours;
    uint32_t minute = tim1->Minutes- tim2->Minutes;
    uint32_t second = tim1->Seconds - tim2->Seconds;
    if(second < 0){
        second += MINUTE_SECOND;
        minute--;
    }
    if(minute < 0){
        minute += HOUR_MINUTE;
        hour--;
    }
    if(hour < 0){
        hour += DAY_HOUR;
    }
    return hour * HOUR_MINUTE * MINUTE_SECOND + minute * MINUTE_SECOND + second;
}


bool RTC_Time_Equal(RTC_TimeTypeDef *tim1, RTC_TimeTypeDef *tim2){
    return (tim1->Hours == tim2->Hours) && (tim1->Minutes == tim2->Minutes) && (tim1->Seconds == tim2->Seconds);
}