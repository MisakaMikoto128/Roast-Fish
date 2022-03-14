#include "UI.h"
#include "rtc.h"
#include "TimeSetting.h"
//@note  You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values

RTC_DateTypeDef sdatestructureget;
RTC_TimeTypeDef stimestructureget;

UI_WigetState_t TimeText_WigetState = UI_WIGET_OFF;
UI_WigetState_t PeriodText_WigetState = UI_WIGET_OFF;
UI_WigetState_t OpenText_WigetState = UI_WIGET_OFF;
UI_WigetState_t CloseText_WigetState = UI_WIGET_OFF;
UI_WigetState_t MinitDotTetx_WigetState = UI_WIGET_OFF;
UI_WigetState_t BoxText_WigetState = UI_WIGET_BLINK;
UI_ClockWigetState_t Clock_WigetState = UI_CLOCKWIGET_ON;

CheckGroup_Wiget_t Mode_CheckGroup_Wiget = {UI_WIGET_OFF, 0, 2, NULL, true};
CheckGroup_Wiget_t Nixie_Wiget_Fodder = {UI_WIGET_OFF, 0, 2, NULL, true};

/**
 * @brief
 *
 * @param wiget
 * @return int current wiget click count
 */
int UI_ClickWiget(CheckGroup_Wiget_t *wiget)
{
    if (wiget->enable)
    {
        if (wiget->state == UI_WIGET_ON)
        {
            if (wiget->click_cnt < wiget->click_cnt_max)
            {
                wiget->click_cnt++;
            }
            else
            {
                wiget->click_cnt = 0;
            }
        }
        else
        {
            wiget->click_cnt = 0;
        }
    }
    return wiget->click_cnt;
}

void UI_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /*Configure GPIO pins : PB0 PB1 PB2 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HT1621Init();
    UI_Constant_Text_Show();
}

/**
 * @brief scan interval is 1ms
 *
 */
void UI_Scan(void)
{

    static uint32_t cnt = 0;
    cnt++;

    if (cnt % 10 == 0)
    {
        Lcd_Show_Updata();
        Sys_Update_State_2_UI();
    }

    if (cnt % 500 == 0)
    {
        switch (Clock_WigetState)
        {
        case UI_WIGET_ON:
            Lcd_Clock_Show(stimestructureget.Hours, stimestructureget.Minutes);
            __TIME_TEXT_On();
            __SECOND_DOT_TEXT_Toggle();
            break;
        case UI_CLOCKWIGET_NONE:
            Lcd_Clock_Show_None();
            __SECOND_DOT_TEXT_On();
            __TIME_TEXT_Off();
            break;
        case UI_CLOCKWIGET_SETTING:
            __TIME_TEXT_Toggle();
            __SECOND_DOT_TEXT_On();
            break;
            case UI_CLOCKWIGET_SHOW_GLOBAL_SETTING_TIME:
            Lcd_Clock_Show(globalSettingTimeObj.Hours, globalSettingTimeObj.Minutes);
            break;
        default:
            break;
        }

        switch (MinitDotTetx_WigetState)
        {
        case UI_WIGET_BLINK:
            __TDOT_TEXT_Toggle();
            break;
        case UI_WIGET_OFF:
            __TDOT_TEXT_Off();
            break;
        case UI_WIGET_ON:
            __TDOT_TEXT_On();
            break;
        default:
            break;
        }

        switch (BoxText_WigetState)
        {
        case UI_WIGET_BLINK:
            __BOX_TEXT_Toggle();
            break;
        case UI_WIGET_OFF:
            __BOX_TEXT_Off();
            break;
        case UI_WIGET_ON:
            __BOX_TEXT_On();
            break;
        default:
            break;
        }

        switch (PeriodText_WigetState)
        {
        case UI_WIGET_BLINK:
            __PERIOD_TEXT_Toggle();
            break;
        case UI_WIGET_OFF:
            __PERIOD_TEXT_Off();
            break;
        case UI_WIGET_ON:
            __PERIOD_TEXT_On();
            break;
        default:
            break;
        }

        switch (OpenText_WigetState)
        {
        case UI_WIGET_BLINK:
            __PERIOD_TEXT_isOn() ? __OPEN_TEXT_On() : __OPEN_TEXT_Off();
            break;
        case UI_WIGET_OFF:
            __OPEN_TEXT_Off();
            break;
        case UI_WIGET_ON:
            __OPEN_TEXT_On();
            break;
        default:
            break;
        }

        switch (CloseText_WigetState)
        {
        case UI_WIGET_BLINK:
            __PERIOD_TEXT_isOn() ? __CLOSE_TEXT_On() : __CLOSE_TEXT_Off();
            break;
        case UI_WIGET_OFF:
            __CLOSE_TEXT_Off();
            break;
        case UI_WIGET_ON:
            __CLOSE_TEXT_On();
            break;
        default:
            break;
        }

        /* Get the RTC current Time */
        HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BCD);
        /* Get the RTC current Date */
        HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BCD);
    }
}

void UI_SendMessage(UI_Message_t message, void *arg)
{
    switch (message)
    {
    case SET_TIME_WIGET_OFF:
        __TIME_TEXT_Off();
        break;
    case SET_CLOCK_SETTING_SHOW:
        Clock_WigetState = UI_CLOCKWIGET_SETTING;
        break;
    case SET_CLOCK_NORMAL_SHOW:
        Clock_WigetState = UI_CLOCKWIGET_ON;
        break;
    case SET_CLOCK_SHOW_NONE:
        Clock_WigetState = UI_CLOCKWIGET_NONE;
        break;
    case SET_CLOCK_SHOW_GLOBAL_SETTING_TIME:
        Clock_WigetState = UI_CLOCKWIGET_SHOW_GLOBAL_SETTING_TIME;
        break;


    case SET_RUN_ON:
        __RUN_TEXT_On();
        __STOP_TEXT_Off();
        break;
    case SET_RUN_OFF:
        __RUN_TEXT_Off();
        __STOP_TEXT_On();
        break;

    case SET_ARROWS_OFF:
        __ARROW1_TEXT_Off();
        __ARROW2_TEXT_Off();
        __ARROW3_TEXT_Off();
        break;
    case SET_ARROW1_ON:
        __ARROW1_TEXT_On();
        __ARROW2_TEXT_Off();
        __ARROW3_TEXT_Off();
        break;
    case SET_ARROW2_ON:
        __ARROW1_TEXT_Off();
        __ARROW2_TEXT_On();
        __ARROW3_TEXT_Off();
        break;
    case SET_ARROW3_ON:
        __ARROW1_TEXT_Off();
        __ARROW2_TEXT_Off();
        __ARROW3_TEXT_On();
        break;

    case SET_MINIT_DOT_BLINK:
        MinitDotTetx_WigetState = UI_WIGET_BLINK;
        break;
    case SET_MINIT_DOT_OFF:
        MinitDotTetx_WigetState = UI_WIGET_OFF;
        break;
    case SET_MINIT_DOT_ON:
        MinitDotTetx_WigetState = UI_WIGET_ON;
        break;

    case SET_OPEN_TEXT_BLINK:
        OpenText_WigetState = UI_WIGET_BLINK;
        break;
    case SET_OPEN_TEXT_OFF:
        OpenText_WigetState = UI_WIGET_OFF;
        break;
    case SET_OPEN_TEXT_ON:
        OpenText_WigetState = UI_WIGET_ON;
        break;
    case SET_CLOSE_TEXT_BLINK:
        CloseText_WigetState = UI_WIGET_BLINK;
        break;
    case SET_CLOSE_TEXT_OFF:
        CloseText_WigetState = UI_WIGET_OFF;
        break;
    case SET_CLOSE_TEXT_ON:
        CloseText_WigetState = UI_WIGET_ON;
        break;

    case SET_PERIOD_TEXT_BLINK:
        PeriodText_WigetState = UI_WIGET_BLINK;
        break;
    case SET_PERIOD_TEXT_OFF:
        PeriodText_WigetState = UI_WIGET_OFF;
        break;
    case SET_PERIOD_TEXT_ON:
        PeriodText_WigetState = UI_WIGET_ON;
        break;

    case SET_PERIOD_NIXIE_VALUE:
        Lcd_Period_Show(*(uint8_t *)arg);
        break;
    case SET_BOX_NIXIE_NONE:
        Lcd_Period_Show(NIXIE_NONE);
        break;
    case SET_BOX_NIXIE_OFF:
        Lcd_Period_Off();
        break;

    case SET_FODDER_NUM:
        Lcd_Fodder_Nixie_Show(*(uint8_t *)arg);
        break;

    case SET_INTERVAL_NUM:
        Lcd_Interval_Nixie_Show(*(uint8_t *)arg);
        break;

    case SET_OUTPUT_NUM:
        Lcd_Output_Nixie_Show(*(uint8_t *)arg);
        break;

    case SET_AREA_NUM:
        Lcd_Area_Nixie_Show(*(uint8_t *)arg);
        break;


    case SET_TIME_TEXT_BLINK:
        TimeText_WigetState = UI_WIGET_BLINK;
        break;
    case SET_TIME_TEXT_OFF:
        TimeText_WigetState = UI_WIGET_OFF;
        break;
    case SET_TIME_TEXT_ON:
        TimeText_WigetState = UI_WIGET_ON;
        break;

    
    default:
        break;
    }
}

void UI_Constant_Text_Show(void)
{
    __FODDER_TEXT_On();
    __AREA_TEXT_On();
    // __SVOT_TEXT_On();
    __MODE_TEXT_On();
    __BOX_TEXT_On();
    __ICON_TEXT_On();
}
