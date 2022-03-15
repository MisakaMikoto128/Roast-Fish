#ifndef _SOFT_WDOG_H_
#define _SOFT_WDOG_H_
#include "main.h"
#include <stdbool.h>
typedef void (*SoftWDOG_Callback)(void);
typedef struct _SoftWDOG
{
    int32_t cnt;
    int32_t upper_limit;
    int32_t lower_limit;
    SoftWDOG_Callback upper_callback;
    SoftWDOG_Callback lower_callback;
    bool enable;
}SoftWDOG;


void SoftWDOG_Init(SoftWDOG *wd,int32_t upper_limit,int32_t lower_limit,SoftWDOG_Callback upper_callback,SoftWDOG_Callback lower_callback);
void SoftWDOG_Feed(SoftWDOG *wd);
void SoftWDOG_Decrease(SoftWDOG *wd);
void SoftWDOG_Reset(SoftWDOG *wd);
void SoftWDOG_Disable(SoftWDOG *wd);

#endif // _SOFT_WDOG_H_