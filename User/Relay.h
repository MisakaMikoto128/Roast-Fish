#ifndef RELAY_H
#define RELAY_H
#include "gpio.h"

#define RELAY_Pin GPIO_PIN_3
#define RELAY_GPIO_Port GPIOB
#define RELAY_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

void Relay_Init(void);
#define RELAY_ON() HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET)
#define RELAY_OFF() HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET)

#endif // RELAY_H