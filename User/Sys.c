#include "Sys.h"
#include "Measure.h"
#include "UI.h"
#include "Counter.h"
#include "TimeSetting.h"
#include "flash.h"
#include "main.h"
#include "Optocoupler.h"
#include "tim.h"
#include "crc.h"
#define HAVE_FODDER_LEVEL GPIO_PIN_SET
#define HAVE_FODDER() (OPTOCOUPLER_Read() == HAVE_FODDER_LEVEL)
Counter fodder_existing_cnt = {.count = 0, .count_max = 3, .count_min = 0, .step = 1};
int32_t vibrator_pluse_delay = VIBRATOR_GET_GEAR_DELAY_TIME(5);
SoftWDOG flashWriteWDOG = {.cnt = 0,
                           .upper_limit = 20,
                           .lower_limit = -20,
                           .upper_callback = NULL,
                           .lower_callback = saveSysStateToFlash,
                           .enable = true};

bool vibrator_enable = false;

PID FishPID;
float piddecayfun(float z)
{
    // return sigmoidabsx(z,11,5);
    // return obliquestepfun(z, 0.0005);
    return 1;
}

void UserPIDInit()
{
    IncPIDInit(&FishPID); // This code is not necessary.
    FishPID.Fmax = 0.85;
    FishPID.Fmin = 0.22;
    FishPID.F = 0.5;
    FishPID.sysArg = PLUS_DELAY_CNT_MAX; // eg. In DC/DC buck control system , this value is PWM output timer's period.
    FishPID.P = 0.004;
    FishPID.I = 0.00005;
    FishPID.D = 0.0002;
    FishPID.Target = 300;
    FishPID.pidDecayByAbsErrorFunc = piddecayfun;
}

Sys sysState = {
    .runState = SYS_STOP,
    .mode = MOD_TIMING,
    .period = {0},
    .fodder_num = 1,
    .interval_num = 1,
    .output_num = 1,
    .area_num = 1,
    .run_time = 0,
    .run_time_set_value = MIN_RUN_TIME,
    .flash_addr = 0x0800F800, // page 31
    .read_count = 0,
    .write_count = 0,
    .crc = 0,
};
Sys sysState_bak = {
    .runState = SYS_STOP,
    .mode = MOD_TIMING,
    .period = {0},
    .fodder_num = 1,
    .interval_num = 1,
    .output_num = 1,
    .area_num = 1,
    .run_time = 0,
    .run_time_set_value = MIN_RUN_TIME,
    .flash_addr = 0x0800F800, // page 31
    .read_count = 0,
    .write_count = 0,
    .crc = 0,
};

// the three points are shown in running and timing mode
// the green LED is the running flag of the vibrator
//@10ms
/**
 * @brief scan interval is 10ms
 *
 */
void Sys_Update_State_2_UI()
{
    if (sysState.runState == SYS_RUN)
    {
        UI_SendMessage(SET_RUN_ON, NULL);
    }
    else
    {
        UI_SendMessage(SET_RUN_OFF, NULL);
    }

    switch (sysState.mode)
    {
    case MOD_TIMING:
        UI_SendMessage(SET_ARROW3_ON, NULL);
        if (sysState.runState == SYS_RUN)
        {
            Lcd_Minit_Nixie_Show(sysState.run_time);
            UI_SendMessage(SET_MINIT_DOT_BLINK, NULL);
        }
        else
        {
            Lcd_Minit_Nixie_Show(sysState.run_time_set_value);
            UI_SendMessage(SET_MINIT_DOT_OFF, NULL);
        }
        __MINIT_TEXT_On();
        break;
    case MOD_NORMAL_OPEN:
        UI_SendMessage(SET_ARROW2_ON, NULL);
        Lcd_Minit_Nixe_Off();
        UI_SendMessage(SET_MINIT_DOT_OFF, NULL);
        __MINIT_TEXT_Off();
        break;
    case MOD_NORMAL_AOTO:
        UI_SendMessage(SET_ARROW1_ON, NULL);
        Lcd_Minit_Nixe_Off();
        UI_SendMessage(SET_MINIT_DOT_OFF, NULL);

        __MINIT_TEXT_Off();
        break;
    default:
        break;
    }

    if (sysState.mode == MOD_NORMAL_AOTO)
    {
        if (CounterGET(&key_period_cnt) == 0)
        {
            UI_SendMessage(SET_PERIOD_TEXT_ON, NULL);
            Lcd_Period_Show(NIXIE_NONE);
            UI_SendMessage(SET_CLOSE_TEXT_OFF, NULL);
            UI_SendMessage(SET_OPEN_TEXT_OFF, NULL);
        }
        else if (IS_ODD(CounterGET(&key_period_cnt)))
        {
            // 1,2->1
            // 3,4->2
            Lcd_Period_Show(CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2);
            UI_SendMessage(SET_CLOSE_TEXT_OFF, NULL);
            UI_SendMessage(SET_PERIOD_TEXT_BLINK, NULL);
            UI_SendMessage(SET_OPEN_TEXT_BLINK, NULL);
        }
        else
        {
            Lcd_Period_Show(CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2);
            UI_SendMessage(SET_OPEN_TEXT_OFF, NULL);
            UI_SendMessage(SET_PERIOD_TEXT_BLINK, NULL);
            UI_SendMessage(SET_CLOSE_TEXT_BLINK, NULL);
        }

        if (sysState.runState == SYS_RUN)
        {
            Counter_reset(&key_period_cnt);
        }
    }
    else
    {
        UI_SendMessage(SET_OPEN_TEXT_OFF, NULL);
        UI_SendMessage(SET_PERIOD_TEXT_OFF, NULL);
        UI_SendMessage(SET_CLOSE_TEXT_OFF, NULL);
        Lcd_Period_Off();
    }

    UI_SendMessage(SET_FODDER_NUM, &sysState.fodder_num);
    UI_SendMessage(SET_INTERVAL_NUM, &sysState.interval_num);
    UI_SendMessage(SET_OUTPUT_NUM, &sysState.output_num);
    UI_SendMessage(SET_AREA_NUM, &sysState.area_num);

    // system time and period time setting
    switch (timeSettingMode)
    {
    case SET_INVALID:
        UI_SendMessage(SET_CLOCK_NORMAL_SHOW, NULL);
        break;
    case SET_RTC_TIME:
        if (Counter_unreach_max(&set_rtc_cnt))
        {
            Counter_increment(&set_rtc_cnt);
            UI_SendMessage(SET_CLOCK_SETTING_SHOW, NULL);
        }
        else
        {
            Counter_reset(&set_rtc_cnt);
            timeSettingMode = SET_INVALID;
        }
        break;
    case SET_PERIOD_START:

        break;
    case SET_PERIOD_END:

        break;

    default:
        break;
    }

    if (sysState.runState == SYS_RUN)
    {
        timeSettingMode = SET_INVALID;
        UI_SendMessage(SET_CLOCK_NORMAL_SHOW, NULL);
        Counter_reset(&key_period_cnt);
    }
    else
    {
    }

    if (HAVE_FODDER())
    {
        __SQUARE_TEXT_On();
        UI_SendMessage(SET_BOX_ON, NULL);
    }
    else
    {
        __SQUARE_TEXT_Off();
        UI_SendMessage(SET_BOX_BLINK, NULL);
    }
};

/**
 * @brief scan interval is 1minute
 *
 */
void Sys_Run_State_Update()
{

    if (sysState.runState == SYS_RUN)
    {
        // TIMING MODE RUNNING
        if (sysState.mode == MOD_TIMING)
        {
            sysState.run_time--;
            if (sysState.run_time == 0)
            {
                sysState.run_time = sysState.run_time_set_value;
                sysState.runState = SYS_STOP;
            }
        }
    }
    else
    {
    }

    if (HAVE_FODDER())
    {
        Counter_reset(&fodder_existing_cnt);
    }
    else
    {
        Counter_increment(&fodder_existing_cnt);
        if (Counter_exceed_or_reach_max(&fodder_existing_cnt))
        {
            sysState.runState = SYS_STOP;
        }
    }
}

/**
 * @brief 1ms scan interval
 *
 */
void Sys_Running_Scan()
{
    static Counter sysRun_cnt = {.count = 0, .count_max = PRE_RUNING_TIME, .count_min = 0, .step = 1};

    if (sysState.runState != sysState_bak.runState)
    {
        sysState_bak.runState = sysState.runState;
        if (sysState.runState == SYS_RUN)
        {
            Counter_reset(&sysRun_cnt);
            Counter_init(&vibrator_interval_time_cnt, GET_FODDER_TIME(sysState.fodder_num),0, 1);
			Counter_init(&vibrator_running_time_cnt, GET_FODDER_TIME(sysState.fodder_num),0, 1);
            HAL_TIM_PWM_Start_IT(&htim14, TIM_CHANNEL_1);
            TIM14->CNT = PLUS_DELAY_CNT_MAX / 2;

            VIBRATOR_ENABLE();
        }
        else
        {
            HAL_TIM_PWM_Stop_IT(&htim14, TIM_CHANNEL_1);
            VIBRATOR_DISABLE();
			FishPID.F = 0.0;
        }
    }

    if (sysState.runState == SYS_RUN)
    {
        if (Counter_unreach_max(&sysRun_cnt))
        {
            Counter_increment(&sysRun_cnt);
            FishPID.Target = PRE_RUNNING_SPEED;
        }
        else
        {
            float F = (FishPID.Fmax - FishPID.Fmin) * (sysState.area_num / (float)MAX_AREA) + FishPID.Fmin;
            FishPID.Target = F * MAX_MOTOR_SPEED;
            VIBRATOR_SET_GEAR_DELAY_TIME(sysState.output_num);
        }

        if(IS_VIBRATOR_ENABLE()){
            Counter_increment(&vibrator_running_time_cnt);
            if(Counter_exceed_or_reach_max(&vibrator_running_time_cnt)){
                VIBRATOR_DISABLE();
                Counter_reset(&vibrator_running_time_cnt);
            }
        }
				
				if(!IS_VIBRATOR_ENABLE()){
            Counter_increment(&vibrator_interval_time_cnt);
            if(Counter_exceed_or_reach_max(&vibrator_interval_time_cnt)){
                VIBRATOR_ENABLE();
                Counter_reset(&vibrator_interval_time_cnt);
            }
        }
    }
}

void reloadSysStateFromFlash()
{
    Sys temp = {0};
    temp = *((Sys *)sysState.flash_addr);
    if(checkSysStateCrcValid(&temp)){
        sysState = temp;
        sysState.read_count++;
    }else{
        saveSysStateToFlash();
    }
}

bool checkSysStateCrcValid(const Sys * pSysState){
    uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)pSysState, SYS_STATE_CRC_LEN);
    return crc == pSysState->crc;
}

void saveSysStateToFlash()
{
	sysState.write_count++;
	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&sysState, SYS_STATE_CRC_LEN);
	sysState.crc = crc;
	
	 // TODO : check sysState.flash_addr is valid
	Flash_Write_Alignment64(sysState.flash_addr,(uint8_t*)&sysState, sizeof(sysState));
	SoftWDOG_Disable(&flashWriteWDOG);
}
