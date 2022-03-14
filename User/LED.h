#ifndef LED_H
#define LED_H
#include "gpio.h"

#define LED_Pin GPIO_Pin_13
#define LED_GPIO_Port GPIOB
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

void LED_Init(void);
#define LED_ON() HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define LED_OFF() HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)

#endif // LED_H