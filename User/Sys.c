#include "Sys.h"
#include "Measure.h"
#include "UI.h"
PID FishPID;
float piddecayfun(float z){
    //return sigmoidabsx(z,11,5);
    //return obliquestepfun(z, 0.0005);
    return 1;
}

void UserPIDInit(){
    IncPIDInit(&FishPID); //This code is not necessary.
    FishPID.Fmax = 0.9;
    FishPID.Fmin = 0.1;
    FishPID.F = 0.5;
    FishPID.sysArg = PLUS_DELAY_CNT_MAX; // eg. In DC/DC buck control system , this value is PWM output timer's period.
    FishPID.P = 0.0004;
    FishPID.I = 0.001;
    FishPID.D = 0.0002;
    FishPID.Target = 1100;
    FishPID.pidDecayByAbsErrorFunc = piddecayfun;
}

Sys sysState = {
    .runState = SYS_RUN,
    .mode = MOD_TIMING,
    .period = {0},
    .fodder_num = 1,
    .interval_num = 1,
    .outPout_num = 1,
    .area_num = 1,
};

void Sys_Update_State_2_UI(){
   if(sysState.runState == SYS_RUN){
       UI_SendMessage(SET_RUN_ON,NULL);
    }else{
       UI_SendMessage(SET_RUN_OFF,NULL);
    }

    switch(sysState.mode){
        case MOD_TIMING:
            UI_SendMessage(SET_ARROW3_ON,NULL);
            break;
        case MOD_NORMAL_OPEN:
            UI_SendMessage(SET_ARROW2_ON,NULL);
            break;
        case MOD_NORMAL_AOTO:
            UI_SendMessage(SET_ARROW1_ON,NULL);
            break;
        default:
            break;
    }
    UI_SendMessage(SET_FODDER_NUM,&sysState.fodder_num);
    UI_SendMessage(SET_INTERVAL_NUM,&sysState.interval_num);
    UI_SendMessage(SET_OUTPUT_NUM,&sysState.outPout_num);
    UI_SendMessage(SET_AREA_NUM,&sysState.area_num);


};


void reloadSysStateFromFlash(){

}