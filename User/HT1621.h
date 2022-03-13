#ifndef __HT1621_H
#define __HT1621_H
#include "main.h"
#include <time.h>
/* �Դ� */
#define LCD_MEM_SZIE 32
extern uint8_t lcd_mem[LCD_MEM_SZIE];

#define SYSOFF 0x00
#define SYSEN  0x01
#define RC256K 0x18
#define BIAS   0x29//ѡ��COMΪ4��������   0x01  ѡ����1/3ƫѹ
#define LCDON  0x03
#define SYSDIS 0x00
#define WDTDIS 0x05
#define WrCmd  0x80//���Ʒ��������ʶ�ĺ�
#define WrData 0xA0//���Ʒ���д���ݵĺ�

#define   PinHigh_WR     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);//��CS���ŵ�ƽ����
#define   PinLow_WR      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);     //��CS���ŵ�ƽ����

#define   PinHigh_Data   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//��CS���ŵ�ƽ����
#define   PinLow_Data    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);     //��CS���ŵ�ƽ����

#define   PinHigh_CS     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);//��CS���ŵ�ƽ����
#define   PinLow_CS      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);     //��CS���ŵ�ƽ����


#define ShiZ_Open()       lcd_mem[1] |=  (1<<3)
#define ShiZ_Togg()       lcd_mem[1] ^=  (1<<3)
#define ShiZ_Close()      lcd_mem[1] &=~ (1<<3)

typedef enum {

//Constant text
FODDER_TEXT,
AREA_TEXT,
SVOT_TEXT,
MODE_TEXT,
BOX_TEXT,
ICON_TEXT,
//Variable text
SQUARE_TEXT,
TIME_TEXT,
RUN_TEXT,
STOP_TEXT,
ARROW1_TEXT,
ARROW2_TEXT,
ARROW3_TEXT,
MINIT_TEXT,
TDOT_TEXT,
PERIOD_TEXT,
OPEN_TEXT,
CLOSE_TEXT,
SECOND_DOT_TEXT,
}LCD_TextName_t;

#define __ICON_TEXT_On()            (lcd_mem[0] |=  (1<<0))
#define __ICON_TEXT_Off()           (lcd_mem[0] &=~ (1<<0))   
#define __ICON_TEXT_Toggle()        (lcd_mem[0] ^=  (1<<0))
#define __TIME_TEXT_On()            (lcd_mem[1] |=  (1<<3))
#define __TIME_TEXT_Off()           (lcd_mem[1] &=~ (1<<3))   
#define __TIME_TEXT_Toggle()        (lcd_mem[1] ^=  (1<<3))
#define __FODDER_TEXT_On()          (lcd_mem[0] |=  (1<<3))
#define __FODDER_TEXT_Off()         (lcd_mem[0] &=~ (1<<3))   
#define __FODDER_TEXT_Toggle()      (lcd_mem[0] ^=  (1<<3))
#define __AREA_TEXT_On()            (lcd_mem[0] |=  (1<<2))
#define __SECOND_DOT_TEXT_On()      (lcd_mem[5] |=  (1<<3))
#define __SECOND_DOT_TEXT_Off()     (lcd_mem[5] &=~ (1<<3))   
#define __SECOND_DOT_TEXT_Toggle()  (lcd_mem[5] ^=  (1<<3))
#define __AREA_TEXT_Off()           (lcd_mem[0] &=~ (1<<2))   
#define __AREA_TEXT_Toggle()        (lcd_mem[0] ^=  (1<<2))
#define __SVOT_TEXT_On()            (lcd_mem[31] |= (1<<3))
#define __SVOT_TEXT_Off()           (lcd_mem[31] &=~(1<<3))   
#define __SVOT_TEXT_Toggle()        (lcd_mem[31] ^= (1<<3))
#define __MODE_TEXT_On()            (lcd_mem[15] |= (1<<1))
#define __MODE_TEXT_Off()           (lcd_mem[15] &=~(1<<1))   
#define __MODE_TEXT_Toggle()        (lcd_mem[15] ^= (1<<1))
#define __BOX_TEXT_On()             (lcd_mem[17] |= (1<<3))
#define __BOX_TEXT_Off()            (lcd_mem[17] &=~(1<<3))   
#define __BOX_TEXT_Toggle()         (lcd_mem[17] ^= (1<<3))
#define __RUN_TEXT_On()             (lcd_mem[15] |= (1<<0))
#define __RUN_TEXT_Off()            (lcd_mem[15] &=~(1<<0))   
#define __RUN_TEXT_Toggle()         (lcd_mem[15] ^= (1<<0))
#define __STOP_TEXT_On()            (lcd_mem[16] |= (1<<0))
#define __STOP_TEXT_Off()           (lcd_mem[16] &=~(1<<0))   
#define __STOP_TEXT_Toggle()        (lcd_mem[16] ^= (1<<0))
#define __SQUARE_TEXT_On()          (lcd_mem[17] |= (1<<2))
#define __SQUARE_TEXT_Off()         (lcd_mem[17] &=~(1<<2))   
#define __SQUARE_TEXT_Toggle()      (lcd_mem[17] ^= (1<<2))
#define __ARROW1_TEXT_On()          (lcd_mem[16] |= (1<<1))
#define __ARROW1_TEXT_Off()         (lcd_mem[16] &=~(1<<1))   
#define __ARROW1_TEXT_Toggle()      (lcd_mem[16] ^= (1<<1))
#define __ARROW2_TEXT_On()          (lcd_mem[16] |= (1<<2))
#define __ARROW2_TEXT_Off()         (lcd_mem[16] &=~(1<<2))   
#define __ARROW2_TEXT_Toggle()      (lcd_mem[16] ^= (1<<2))
#define __ARROW3_TEXT_On()          (lcd_mem[16] |= (1<<3))
#define __ARROW3_TEXT_Off()         (lcd_mem[16] &=~(1<<3))   
#define __ARROW3_TEXT_Toggle()      (lcd_mem[16] ^= (1<<3))
#define __MINIT_TEXT_On()           (lcd_mem[15] |= (1<<3))
#define __MINIT_TEXT_Off()          (lcd_mem[15] &=~(1<<3))   
#define __MINIT_TEXT_Toggle()       (lcd_mem[15] ^= (1<<3))
#define __TDOT_TEXT_On()            (lcd_mem[15] |= (1<<2))
#define __TDOT_TEXT_Off()           (lcd_mem[15] &=~(1<<2))   
#define __TDOT_TEXT_Toggle()        (lcd_mem[15] ^= (1<<2))
#define __PERIOD_TEXT_On()          (lcd_mem[10] |= (1<<3))
#define __PERIOD_TEXT_Off()         (lcd_mem[10] &=~(1<<3))   
#define __PERIOD_TEXT_Toggle()      (lcd_mem[10] ^= (1<<3))
#define __OPEN_TEXT_On()            (lcd_mem[17] |= (1<<1))
#define __OPEN_TEXT_Off()           (lcd_mem[17] &=~(1<<1))   
#define __OPEN_TEXT_Toggle()        (lcd_mem[17] ^= (1<<1))
#define __CLOSE_TEXT_On()           (lcd_mem[0] |=  (1<<1))
#define __CLOSE_TEXT_Off()          (lcd_mem[0] &=~ (1<<1))   
#define __CLOSE_TEXT_Toggle()       (lcd_mem[0] ^=  (1<<1))

#define __PERIOD_TEXT_isOn()        (lcd_mem[10] & (1<<3))


void WriteDatatoHT1621(uint8_t Data,uint8_t num);
void WritetoHT1621(uint8_t Addr,uint8_t Data,uint8_t Mode);
void WriteCmdtoHT1621(uint8_t Cmd);
void HT1621Init(void);

void Lcd_Text_On(LCD_TextName_t textName);
void Lcd_Text_Off(LCD_TextName_t textName);
void Lcd_Text_Toggle(LCD_TextName_t textName);

void Lcd_8Nixie_ShowChar(int id, char c);
void Lcd_Clock_Show_None();
void Lcd_Period_Show(uint8_t period);
void Lcd_Period_Off();
void Lcd_Minit_Nixie_Show(uint8_t minit);
void Lcd_Fodder_Nixie_Show(uint8_t num);
void Lcd_Interval_Nixie_Show(uint8_t num);
void Lcd_Output_Nixie_Show(uint8_t num);
void Lcd_Area_Nixie_Show(uint8_t num);
void Lcd_Supply_Nixie_Show(uint16_t num);

void Lcd_8Nixie_Off(int id);
void Lcd_10Nixe_Off(const uint8_t *mem_addr);
void Lcd_Minit_Nixe_Off();
#define NIXIE_NONE -1
void Lcd_Clock_Show(uint8_t hour,uint8_t minute);
void Lcd_Show_Updata(void);




#endif


