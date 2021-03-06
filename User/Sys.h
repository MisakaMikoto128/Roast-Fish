#ifndef _SYS_H_
#define _SYS_H_
#define PLUS_DELAY_CNT_MAX (64000)
#define MAX_MOTOR_SPEED (1200)
#define PRE_RUNING_TIME 3000 //ms //TODO: change to 4000 ms
#define PRE_RUNNING_SPEED 1200 //Hz
#define TIMER1_FREQ 400000
#define TIMER3_FREQ 100L
#include "PID.h"
#include "rtc.h"
#include "main.h"
#include <time.h>
#include <stdbool.h>
#include "Counter.h"
#include "SoftWDOG.h"
#include "LED.h"

extern PID FishPID;
void UserPIDInit();

#define MAX_FODDER_TIME 6000 //ms 
#define MAX_FODDER 16
#define MIN_FODDER 1
#define GET_FODDER_TIME(x) ((x)/(float)MAX_FODDER * MAX_FODDER_TIME)

#define MAX_INTERVAL_TIME 18000UL //ms 18s
#define MAX_INTERVAL 16
#define MIN_INTERVAL 1
#define GET_INTERVAL_TIME(x) ((x)/(float)MAX_INTERVAL * MAX_INTERVAL_TIME)

#define MAX_OUTPOUT 16
#define MIN_OUTPOUT 1

#define MAX_AREA 16
#define MIN_AREA 1

#define MODE_NUM 3
#define PERIOD_NUM 8
#define RUN_STATE_NUM 2

#define MIN_RUN_TIME 5


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
    int output_num;
    int area_num;
    int run_time_set_value;
    int run_time; //定时模式的运行时间
    uint32_t flash_addr;
    uint32_t write_count;
    uint32_t read_count;
    uint32_t crc;
}Sys;

#define SYS_STATE_CRC_LEN (sizeof(Sys) - sizeof(uint32_t))


extern Sys sysState;
extern Sys sysState_bak;
extern bool vibrator_enable;

#define VIBRATOR_ENABLE() do{vibrator_enable = true;LED_ON();}while(0)
#define VIBRATOR_DISABLE() do{vibrator_enable = false;LED_OFF();}while(0)
#define IS_VIBRATOR_ENABLE() (vibrator_enable)
#define VIBRATOR_DELAY_MAX 8000UL //Hz
#define VIBRATOR_GEAR_NUM 16UL
#define VIBRATOR_GEAR_MIN 3000UL //Hz
#define VIBRATOR_GEAR_MAX (VIBRATOR_DELAY_MAX - VIBRATOR_GEAR_MIN)
#define VIBRATOR_GEAR_STEP ((VIBRATOR_GEAR_MAX - VIBRATOR_GEAR_MIN)/(float)VIBRATOR_GEAR_NUM)
#define VIBRATOR_GET_GEAR_DELAY_TIME(x)	(VIBRATOR_GEAR_MIN+VIBRATOR_GEAR_STEP*(x)) //x = 0-16
extern int32_t vibrator_pluse_delay;
#define VIBRATOR_SET_GEAR_DELAY_TIME(x) (vibrator_pluse_delay = VIBRATOR_GET_GEAR_DELAY_TIME(x))
void Sys_Update_State_2_UI();
void Sys_Run_State_Update();
void Sys_Running_Scan();
void Sys_Running_Schedule_Scan();
void reloadSysStateFromFlash();
void saveSysStateToFlash();
bool checkSysStateCrcValid(const Sys * pSysState);
extern Counter key_calitime_cnt;
extern Counter key_mode_cnt;
extern Counter key_fodder_cnt;
extern Counter key_interval_cnt;
extern Counter key_output_cnt;
extern Counter key_area_cnt;
extern Counter key_run_time_set_cnt;
extern Counter key_period_cnt;
extern Counter set_rtc_cnt;
extern Counter vibrator_interval_time_cnt;
extern Counter vibrator_running_time_cnt ;


extern SoftWDOG flashWriteWDOG;
#endif // _SYS_H_