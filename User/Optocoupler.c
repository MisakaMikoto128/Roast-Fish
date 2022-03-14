#include "Optocoupler.h"

void Optocoupler_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    OPTOCOUPLER_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = OPTOCOUPLER_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OPTOCOUPLER_GPIO_Port, &GPIO_InitStruct);
}
