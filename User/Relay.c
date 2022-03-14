#include "Relay.h"

void Relay_Init(void){
    
    GPIO_InitTypeDef GPIO_InitStruct;
    RELAY_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = RELAY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RELAY_GPIO_Port, &GPIO_InitStruct);
}