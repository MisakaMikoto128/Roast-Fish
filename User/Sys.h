#ifndef _SYS_H_
#define _SYS_H_
#define PLUS_DELAY_CNT_MAX (64000)
#define TIMER1_FREQ 400000
#define TIMER3_FREQ 100L
#include "PID.h"
#include "rtc.h"
#include "main.h"
#include <time.h>
#include <stdbool.h>
#include "Counter.h"
#include "SoftWDOG.h"
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

#define MIN_RUN_TIME 5

#define PRE_RUNING_TIME 3000 //ms

#define PRE_RUNNING_SPEED 300 //Hz
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
#define VIBRATOR_ENABLE() (vibrator_enable = true)
#define VIBRATOR_DISABLE() (vibrator_enable = false)
#define IS_VIBRATOR_ENABLE() (vibrator_enable)
#define VIBRATOR_DELAY_MAX 10000UL
#define VIBRATOR_GEAR_NUM 16UL
#define VIBRATOR_GEAR_MIN 200UL
#define VIBRATOR_GEAR_MAX (VIBRATOR_DELAY_MAX - VIBRATOR_GEAR_MIN)
#define VIBRATOR_GEAR_STEP ((VIBRATOR_GEAR_MAX - VIBRATOR_GEAR_MIN)/VIBRATOR_GEAR_NUM)
#define VIBRATOR_GET_GEAR_DELAY_TIME(x)	(VIBRATOR_GEAR_MIN+VIBRATOR_GEAR_STEP*(x)) //x = 0-16
extern int32_t vibrator_pluse_delay;
void Sys_Update_State_2_UI();
void Sys_Run_State_Update();
void Sys_Running_Scan();
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


extern SoftWDOG flashWriteWDOG;
#endif // _SYS_H_