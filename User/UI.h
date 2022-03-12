#ifndef _UI_H_
#define _UI_H_

#include "main.h"
#include "HT1621.h"
typedef enum{
    UI_WIGET_ON,
    UI_WIGET_OFF,
    UI_WIGET_BLINK,
}UI_WigetState_t;

typedef enum{
    UI_CLOCKWIGET_ON,
    UI_CLOCKWIGET_OFF,
    UI_CLOCKWIGET_BLINK,
    UI_CLOCKWIGET_SETTING,
    UI_CLOCKWIGET_NONE,
}UI_ClockWigetState_t;

typedef enum{
	SET_TIME_WIGET_OFF,



    SET_CLOCK_NORMAL_SHOW,
    SET_CLOCK_SETTING_SHOW,
    SET_CLOCK_SHOW_NONE,

}UI_Message_t;

void UI_Init(void);
void UI_Scan(void);
void UI_SendMessage(UI_Message_t message);
void UI_Constant_Text_Show(void);


#endif // _UI_H_


