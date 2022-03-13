#ifndef _TIME_SETTING_H_
#define _TIME_SETTING_H_
#include "rtc.h"

typedef enum
{
    SET_INVALID,
    SET_RTC_TIME,
    SET_PERIOD_START,
    SET_PERIOD_END,
} TimeSettingMode_t;

extern TimeSettingMode_t timeSettingMode;
extern RTC_TimeTypeDef globalSettingTimeObj;
#endif // _TIME_SETTING_H_