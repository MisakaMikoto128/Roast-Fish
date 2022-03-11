#ifndef _SYS_H_
#define _SYS_H_
#define PLUS_DELAY_CNT_MAX (64000)
#define TIMER1_FREQ 400000
#define TIMER3_FREQ 100L
#include "PID.h"
extern PID FishPID;
void UserPIDInit();
#endif // _SYS_H_