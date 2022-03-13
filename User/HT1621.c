#include "HT1621.h"

uint8_t lcd_mem[LCD_MEM_SZIE] = {0};

// Nixie tube code definition
const uint8_t num_left_table[] = {0x05, 0x05, 0x06, 0x07, 0x07, 0x03, 0x03, 0x05, 0x07, 0x07, 0x02};
const uint8_t num_right_table[] = {0x0F, 0x00, 0x0B, 0x09, 0x04, 0x0D, 0x0F, 0x08, 0x0F, 0x0D};
typedef enum
{
    Nixie7,
    Nixie6,
} NixieType;

typedef struct Nxie_t_
{
    NixieType type;
    uint8_t memid1;
    uint8_t memid2;
} Nxie_t;

const uint8_t Nixie_Mem_ID_table[][2] = {{7, 8}, {5, 6}, {3, 4}, {1, 2}, {10, 9}, {12, 11}, {14, 13}, {31, 30}, {29, 28}, {27, 26}};

const uint8_t Clock_Nixie_Mem_ID_table[][2] = {{7, 8}, {5, 6}, {3, 4}, {1, 2}, {10, 9}};
const uint8_t Period_Nixie_Mem_ID_table[][2] = {{10, 9}};
const uint8_t Minit_Nixie_Mem_ID_table[][2] = {{12,11},{14, 13}};
const uint8_t Vot_Nixie_Mem_ID_table[][2] = {{27, 26}, {29, 28}, {31, 30}};
const uint8_t Fodder_Nixie_Mem_ID_table[][2] = {{19, 18}};
const uint8_t Interval_Nixie_Mem_ID_table[][2] = {{21, 20}};
const uint8_t Output_Nixie_Mem_ID_table[][2] = {{23, 22}};
const uint8_t Area_Nixie_Mem_ID_table[][2] = {{25, 24}};
//{30,31},{29,28},{27,26},{25,24},{23,22},{21,20},
/*
lcd_mem[0]: Block1 -> bit:
*/

void WriteDatatoHT1621(uint8_t Data, uint8_t num)
{
    uint8_t i;
    for (i = 0; i < num; i++)
    {
        PinLow_WR; //����WR��  Data���ϵ�������WR��λ������д��

        if (((Data & 0x80) >> 7) == 1) //�����ǰbitΪ1��������DATA����
            PinHigh_Data;
        if (((Data & 0x80) >> 7) == 0) //�����ǰbitΪ0��������DATA����
            PinLow_Data;

        PinHigh_WR; //��ǰ��һbitд�꣬����WR�ţ�Ϊ�´�WRΪ��������׼��
        Data <<= 1; //��ǰλ�����ˣ��Ƶ���һλ������������
    }
}

/*******************************************************************************
 * fuction	WritetoHT1621
 * brief	��Ҫ��ʾ������д��HT1621
 * param	Addr��д���ʼ��ַ��Data��д�����ݣ�Mode��0x80 /0xa0
 * return	��
 *******************************************************************************/
void WritetoHT1621(uint8_t Addr, uint8_t Data, uint8_t Mode)
{
    PinLow_CS;
    WriteDatatoHT1621(Mode, 3);      //д��0x80 /0xa0
    WriteDatatoHT1621(Addr << 2, 6); //д���ַ
    WriteDatatoHT1621(Data << 4, 4); //д������  �����Ƕ�Ӧ����SEGд�� �ʶ�Ϊ4
    PinHigh_CS;
}

/*******************************************************************************
 * fuction	WriteCmdtoHT1621
 * brief	д���HT1621
 * param	Cmd������
 * return	��
 *******************************************************************************/
void WriteCmdtoHT1621(uint8_t Cmd)
{
    PinLow_CS;                   //����CS��  CS����ʱWR��Ч
    WriteDatatoHT1621(WrCmd, 3); //д�������־100    0x80 3
    WriteDatatoHT1621(Cmd, 8);   //д����������
    WriteDatatoHT1621(0, 1);
    PinHigh_CS; //����CS��
}

void HT1621Init(void)
{
    PinHigh_CS;
    PinHigh_WR;
    PinHigh_Data;

    /* ����HT1621 */
    WriteCmdtoHT1621(SYSEN);
    WriteCmdtoHT1621(RC256K);
    WriteCmdtoHT1621(BIAS);
    WriteCmdtoHT1621(LCDON);

    /* �����Ļ */
    for (int i = 0; i < LCD_MEM_SZIE; i++)
    {
        WritetoHT1621(i, 0x00, WrData);
    }
}

void Lcd_Text_On(LCD_TextName_t textName)
{
    switch (textName)
    {
    case TIME_TEXT:
        __TIME_TEXT_On();
        break;
    case AREA_TEXT:
        __AREA_TEXT_On();
        break;
    case FODDER_TEXT:
        __FODDER_TEXT_On();
        break;
    case SVOT_TEXT:
        __SVOT_TEXT_On();
        break;
    case MODE_TEXT:
        __MODE_TEXT_On();
        break;
    case BOX_TEXT:
        __BOX_TEXT_On();
        break;
    case RUN_TEXT:
        __RUN_TEXT_On();
        break;
    case STOP_TEXT:
        __STOP_TEXT_On();
        break;
    case SQUARE_TEXT:
        __SQUARE_TEXT_On();
        break;
    case ARROW1_TEXT:
        __ARROW1_TEXT_On();
        break;
    case ARROW2_TEXT:
        __ARROW2_TEXT_On();
        break;
    case ARROW3_TEXT:
        __ARROW3_TEXT_On();
        break;
    case MINIT_TEXT:
        __MINIT_TEXT_On();
        break;
    case TDOT_TEXT:
        __TDOT_TEXT_On();
        break;
    case PERIOD_TEXT:
        __PERIOD_TEXT_On();
        break;
    case OPEN_TEXT:
        __OPEN_TEXT_On();
        break;
    case CLOSE_TEXT:
        __CLOSE_TEXT_On();
        break;
    case SECOND_DOT_TEXT:
        __SECOND_DOT_TEXT_On();
        break;
    case ICON_TEXT:
        __ICON_TEXT_On();

        break;
    default:
        break;
    }
}

void Lcd_Text_Off(LCD_TextName_t textName);
void Lcd_Text_Toggle(LCD_TextName_t textName);
void Lcd_Show_Updata(void)
{
    for (int i = 0; i < LCD_MEM_SZIE; i++)
        WritetoHT1621(i, lcd_mem[i], WrData);
}

void Lcd_8Nixie_ShowChar(int id, char c)
{
    if (id >= sizeof(Nixie_Mem_ID_table) / 2 / sizeof(uint8_t))
        return;
    if ((c >= '0' && c <= '9'))
    {
        char num = c - '0';
        lcd_mem[Nixie_Mem_ID_table[id][0]] = num_left_table[num] | (lcd_mem[Nixie_Mem_ID_table[id][0]] & 0x08);
        lcd_mem[Nixie_Mem_ID_table[id][1]] = num_right_table[num];
    }
    else
    {
        lcd_mem[Nixie_Mem_ID_table[id][0]] = num_left_table[sizeof(num_left_table) - 1] | (lcd_mem[Nixie_Mem_ID_table[id][0]] & 0x08);
        lcd_mem[Nixie_Mem_ID_table[id][1]] = 0x00;
    }
}

/**
 * @brief 
 * 
 * @param id 
 * @param num num == NIXIE_NONE,show '-'
 */
void Lcd_8Nixie_ShowNum(int id, uint8_t num)
{
    if (id >= sizeof(Nixie_Mem_ID_table) / 2 / sizeof(uint8_t))
        return;
    if (num >= 0 && num <= 9)
    {
        lcd_mem[Nixie_Mem_ID_table[id][0]] = num_left_table[num] | (lcd_mem[Nixie_Mem_ID_table[id][0]] & 0x08);
        lcd_mem[Nixie_Mem_ID_table[id][1]] = num_right_table[num];
    }
    else
    {
        lcd_mem[Nixie_Mem_ID_table[id][0]] = num_left_table[sizeof(num_left_table) - 1] | (lcd_mem[Nixie_Mem_ID_table[id][0]] & 0x08);
        lcd_mem[Nixie_Mem_ID_table[id][1]] = 0x00;
    }
}


/**
 * @brief 
 * 
 * @param id 
 * @param num num > 9,show '1' at the left
 */
void Lcd_10Nixie_ShowNum(uint8_t num, const uint8_t *mem_addr)
{

    if (num >= 0 && num <= 9)
    {
        lcd_mem[mem_addr[0]] = num_left_table[num] & (~0x08);
        lcd_mem[mem_addr[1]] = num_right_table[num];
    }
    else
    {
        lcd_mem[mem_addr[0]] = num_left_table[num % 10] | (0x08);
        lcd_mem[mem_addr[1]] = num_right_table[num % 10];
    }
}

void Lcd_8Nixie_Off(int id){
    lcd_mem[Nixie_Mem_ID_table[id][0]] = 0x00;
    lcd_mem[Nixie_Mem_ID_table[id][1]] = 0x00;
}

void Lcd_10Nixe_Off(const uint8_t *mem_addr){
    lcd_mem[mem_addr[0]] =0x00;
    lcd_mem[mem_addr[1]] = 0x00;
}

void Lcd_Clock_Show(uint8_t hour, uint8_t minute)
{
    /* Сʱ��ʾ */
    Lcd_8Nixie_ShowNum(0, hour / 10);
    Lcd_8Nixie_ShowNum(1, hour % 10);
    /* ������ʾ */
    Lcd_8Nixie_ShowNum(2, minute / 10);
    Lcd_8Nixie_ShowNum(3, minute % 10);
}

void Lcd_Clock_Show_None()
{
    /* Сʱ��ʾ */
    Lcd_8Nixie_ShowNum(0, '-');
    Lcd_8Nixie_ShowNum(1, '-');
    /* ������ʾ */
    Lcd_8Nixie_ShowNum(2, '-');
    Lcd_8Nixie_ShowNum(3, '-');
}

void Lcd_Period_Show(uint8_t period)
{
    Lcd_8Nixie_ShowNum(4, period);
}

void Lcd_Period_Off()
{
    Lcd_8Nixie_Off(4);
}

void Lcd_Minit_Nixie_Show(uint8_t minit)
{
    if (minit < 200)
    { // this nixie group can only show number in 0-199
			Lcd_10Nixie_ShowNum(minit % 10,&Minit_Nixie_Mem_ID_table[1][0]);
			minit /= 10;
			Lcd_10Nixie_ShowNum(minit, &Minit_Nixie_Mem_ID_table[0][0]);
    }
}

inline void Lcd_Fodder_Nixie_Show(uint8_t num)
{
    Lcd_10Nixie_ShowNum(num, &Fodder_Nixie_Mem_ID_table[0][0]);
}

inline void Lcd_Interval_Nixie_Show(uint8_t num)
{
    Lcd_10Nixie_ShowNum(num, &Interval_Nixie_Mem_ID_table[0][0]);
}

inline void Lcd_Output_Nixie_Show(uint8_t num)
{
    Lcd_10Nixie_ShowNum(num, &Output_Nixie_Mem_ID_table[0][0]);
}

inline void Lcd_Area_Nixie_Show(uint8_t num)
{
    Lcd_10Nixie_ShowNum(num, &Area_Nixie_Mem_ID_table[0][0]);
}

void Lcd_Supply_Nixie_Show(uint16_t num)
{
    Lcd_8Nixie_ShowNum(7,num%10);
    num/=10;
    Lcd_8Nixie_ShowNum(8,num%10);
    num/=10;
    Lcd_8Nixie_ShowNum(9,num%10);
}

void Lcd_Minit_Nixe_Off(){
    Lcd_10Nixe_Off(&Minit_Nixie_Mem_ID_table[0][0]);
	Lcd_10Nixe_Off(&Minit_Nixie_Mem_ID_table[1][0]);
}