#ifndef _SYS_H_
#define _SYS_H_
#define PLUS_DELAY_CNT_MAX (64000)
#define TIMER1_FREQ 400000
#define TIMER3_FREQ 100L
#include "PID.h"
#include "rtc.h"
#include <time.h>
#include <stdbool.h>
extern PID FishPID;
void UserPIDInit();

#define MAX_FODDER 15
#define MIN_FODDER 0

#define MAX_INTERVAL 15
#define MIN_INTERVAL 0

#define MAX_OUTPOUT 15
#define MIN_OUTPOUT 0

#define MAX_AREA 15
#define MIN_AREA 0

#define MODE_NUM 3
#define PERIOD_NUM 8
#define RUN_STATE_NUM 2

typedef enum {
    SYS_RUN,
    SYS_STOP,
}SysRunState;

typedef enum {
    MOD_TIMING,
    MOD_NORMAL_OPEN,
    MOD_NORMAL_AOTO,
}SysMode;


typedef struct SysPeriodNode_
{
    RTC_TimeTypeDef start;
    RTC_TimeTypeDef end;
    bool isOpen;
}SysPeriodNode;


typedef struct Sys_
{
    SysRunState runState;
    SysMode mode;
    SysPeriodNode period[PERIOD_NUM];
    int fodder_num;
    int interval_num;
    int outPout_num;
    int area_num;
}Sys;

extern Sys sysState;

void Sys_Update_State_2_UI();
void reloadSysStateFromFlash();
#endif // _SYS_H_