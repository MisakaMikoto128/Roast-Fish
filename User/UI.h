#ifndef _UI_H_
#define _UI_H_

#include "main.h"
#include "HT1621.h"
#include <stdbool.h>
typedef enum
{
    UI_WIGET_ON = 0,
    UI_WIGET_OFF,
    UI_WIGET_BLINK,
} UI_WigetState_t;

typedef enum
{
    UI_CLOCKWIGET_ON,
    UI_CLOCKWIGET_OFF,
    UI_CLOCKWIGET_BLINK,
    UI_CLOCKWIGET_SETTING,
    UI_CLOCKWIGET_NONE,
} UI_ClockWigetState_t;

typedef enum
{
    SET_TIME_WIGET_OFF,

    SET_CLOCK_NORMAL_SHOW,
    SET_CLOCK_SETTING_SHOW,
    SET_CLOCK_SHOW_NONE,

    SET_RUN_ON,
    SET_RUN_OFF,

    SET_ARROWS_OFF,
    SET_ARROW1_ON,
    SET_ARROW2_ON,
    SET_ARROW3_ON,
    SET_FODDER_NUM,
    SET_INTERVAL_NUM,
    SET_OUTPUT_NUM,
    SET_AREA_NUM,

    SET_MINIT_DOT_BLINK,
    SET_MINIT_DOT_OFF,
    SET_MINIT_DOT_ON,
} UI_Message_t;

typedef struct CheckGroup_Wiget_
{
    UI_WigetState_t state;
    int click_cnt;
    int click_cnt_max;
    void (*onClick)(int click_cnt);
    bool enable;
} CheckGroup_Wiget_t;

void UI_Init(void);
void UI_Scan(void);
void UI_SendMessage(UI_Message_t message,void * arg);
void UI_Constant_Text_Show(void);
int ClickWiget(CheckGroup_Wiget_t *wiget);

#endif // _UI_H_
