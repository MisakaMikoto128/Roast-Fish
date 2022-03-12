#include "UI.h"



void UI_Init(void){

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /*Configure GPIO pins : PB0 PB1 PB2 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
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
void UI_Scan(void){

    static uint32_t cnt = 0;
    cnt++;

    if(cnt % 10 == 0){
        Lcd_Show_Updata();
    }


    if(cnt % 500 == 0){
        
    }




}


void UI_SendMessage(uint8_t *pMsg){





}

void UI_Constant_Text_Show(void){
    __FODDER_TEXT_On();
    __AREA_TEXT_On();
   // __SVOT_TEXT_On();
    __MODE_TEXT_On();
    __BOX_TEXT_On();
    __ICON_TEXT_On();
}


