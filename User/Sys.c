#include "Sys.h"
#include "Measure.h"
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


SysRunState sysRunState = SYS_RUN;
SysMode sysMode = MOD_TIMING;
