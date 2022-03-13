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
    .output_num = 1,
    .area_num = 1,
    .run_time = 0,
    .run_time_set_value = MIN_RUN_TIME
};
//三个点是在运行和定时模式下出现的
//绿色LED是振动器运行标志
void Sys_Update_State_2_UI(){
   if(sysState.runState == SYS_RUN){
       UI_SendMessage(SET_RUN_ON,NULL);
    }else{
       UI_SendMessage(SET_RUN_OFF,NULL);
    }

    switch(sysState.mode){
        case MOD_TIMING:
            UI_SendMessage(SET_ARROW3_ON,NULL);
            if(sysState.runState == SYS_RUN){
                Lcd_Minit_Nixie_Show(sysState.run_time);
                UI_SendMessage(SET_MINIT_DOT_BLINK,NULL);
            }else{
                Lcd_Minit_Nixie_Show(sysState.run_time_set_value);
                UI_SendMessage(SET_MINIT_DOT_OFF,NULL);
            }
            __MINIT_TEXT_On();
            break;
        case MOD_NORMAL_OPEN:
            UI_SendMessage(SET_ARROW2_ON,NULL);
            Lcd_Minit_Nixe_Off();
            UI_SendMessage(SET_MINIT_DOT_OFF,NULL);
            __MINIT_TEXT_Off();
            break;
        case MOD_NORMAL_AOTO:
            UI_SendMessage(SET_ARROW1_ON,NULL);
            Lcd_Minit_Nixe_Off();
            UI_SendMessage(SET_MINIT_DOT_OFF,NULL);
            __MINIT_TEXT_Off();
            break;
        default:
            break;
    }
    UI_SendMessage(SET_FODDER_NUM,&sysState.fodder_num);
    UI_SendMessage(SET_INTERVAL_NUM,&sysState.interval_num);
    UI_SendMessage(SET_OUTPUT_NUM,&sysState.output_num);
    UI_SendMessage(SET_AREA_NUM,&sysState.area_num);

    

};


void reloadSysStateFromFlash(){

}