#ifndef __FLASH_H
#define __FLASH_H
#include "main.h"
#include "stm32g0xx_hal_flash.h"
void FLASH_Write_Test();
 void FLASH_Page_Erase(uint32_t page);
 void Flash_Write_Alignment64(uint32_t address, const uint8_t *data, uint32_t size);
#endif 