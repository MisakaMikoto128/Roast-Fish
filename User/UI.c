#include "UI.h"
#include "rtc.h"
//@note  You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values

RTC_DateTypeDef sdatestructureget;
RTC_TimeTypeDef stimestructureget;

UI_WigetState_t TimeText_WigetState = UI_WIGET_OFF;
UI_WigetState_t PeriodText_WigetState = UI_WIGET_OFF;
UI_WigetState_t OpenText_WigetState = UI_WIGET_OFF;
UI_WigetState_t CloseText_WigetState = UI_WIGET_OFF;
UI_ClockWigetState_t Clock_WigetState = UI_CLOCKWIGET_ON;
struct Mode_CheckGroup_Wiget_
{
  UI_WigetState_t state;
  int click_cnt;
  int click_cnt_max;
}Mode_CheckGroup_Wiget = {UI_WIGET_OFF, 0, 2};


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
    }

    if (cnt % 500 == 0)
    {
        switch (Clock_WigetState)
        {
        case UI_CLOCKWIGET_ON:
            Lcd_Clock_Show(stimestructureget.Hours, stimestructureget.Seconds);
            __TIME_TEXT_On();
            __SECOND_DOT_TEXT_Toggle();
            break;
        case UI_CLOCKWIGET_NONE:
            Lcd_Clock_Show_None();
            __SECOND_DOT_TEXT_On();
            break;
        case UI_CLOCKWIGET_SETTING:
            __TIME_TEXT_Toggle();
            __SECOND_DOT_TEXT_On();
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

void UI_SendMessage(UI_Message_t message)
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
        default:
        break;
    }
}


void UI_Click_Wiget(){

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
