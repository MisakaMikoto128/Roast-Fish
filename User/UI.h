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


}UI_Message_t;

void UI_Init(void);
void UI_Scan(void);
void UI_SendMessage(uint8_t *pMsg);
void UI_Constant_Text_Show(void);


#endif // _UI_H_



