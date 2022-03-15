#include "SoftWDOG.h"

void SoftWDOG_Init(SoftWDOG *wd,int32_t upper_limit,int32_t lower_limit,SoftWDOG_Callback upper_callback,SoftWDOG_Callback lower_callback)
{
    wd->cnt = 0;
    wd->upper_limit = upper_limit;
    wd->lower_limit = lower_limit;
    wd->upper_callback = upper_callback;
    wd->lower_callback = lower_callback;
    wd->enable = true;
}

void SoftWDOG_Feed(SoftWDOG *wd){
    if(wd->cnt < INT_MAX){
        wd->cnt++;
    }
    if(wd->cnt >= wd->upper_limit){
        if(wd->upper_callback != NULL && wd->enable){
            wd->upper_callback();
        }
    }
}
void SoftWDOG_Decrease(SoftWDOG *wd){
    if(wd->cnt > INT_MIN){
        wd->cnt--;
    }
    if(wd->cnt <= wd->lower_limit){
        if(wd->lower_callback != NULL && wd->enable){
            wd->lower_callback();
        }
    }
}
inline void SoftWDOG_Reset(SoftWDOG *wd){
    wd->cnt = 0;
    wd->enable = true;
}
inline void SoftWDOG_Disable(SoftWDOG *wd){
    wd->enable = false;
}