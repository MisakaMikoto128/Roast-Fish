#include "flash.h"
#include <string.h>
#define DAFALT_FLASH_ERASE_TIMEOUT (1000UL)
/**
 * @brief  Gets the page of a given address
 * @param  Addr: Address of the FLASH Memory
 * @retval The page of a given address
 */
static uint32_t GetPage(uint32_t Addr)
{
    return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    ;
}

/**
 * @brief  Erase the specified FLASH memory page.
 * @param  Page FLASH page to erase
 *         This parameter must be a value between 0 and (max number of pages in Flash - 1)
 * @note (*) availability depends on devices,Bank1 to be erased.
 * @retval None
 */
void FLASH_PageErase_L(uint32_t Page)
{
    uint32_t tmp;

    /* Check the parameters */
    assert_param(IS_FLASH_PAGE(Page));

    /* Get configuration register, then clear page number */
    tmp = (FLASH->CR & ~FLASH_CR_PNB);

    /* Set page number, Page Erase bit & Start bit */
    FLASH->CR = (tmp | (FLASH_CR_STRT | (Page << FLASH_CR_PNB_Pos) | FLASH_CR_PER));
}

HAL_StatusTypeDef Flash_Write64(uint32_t address, uint64_t data)
{
    HAL_StatusTypeDef status = HAL_OK;
    HAL_FLASH_Unlock();
    status = HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, address, data);
    HAL_FLASH_Lock();
    return status;
}
/**
 * @brief Write a buffer to the FLASH memory.Aligment of 64 bits. if data size is not multiple of 64 bits, the remaining bits will be extended with 0.
 * 
 * @param address flash address
 * @param data byte array to write
 * @param size byte array size
 */
void Flash_Write_Alignment64(uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t row = size/sizeof(uint64_t);
    uint32_t res = size%sizeof(uint64_t);
    uint32_t page = GetPage(address);
    uint64_t tmp = 0;
    FLASH_Page_Erase(page);
    HAL_FLASH_Unlock();
    for(uint32_t i = 0; i < row; i++)
    {
        HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, address, *(uint64_t*)data);
        address += sizeof(uint64_t);
        data += sizeof(uint64_t);
    }
    if(res)
    {
        
        memcpy(&tmp, data, res);
        HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, address, tmp);
    }
    HAL_FLASH_Lock();
}

void FLASH_Page_Erase(uint32_t page)
{
    HAL_StatusTypeDef status = HAL_OK;
    HAL_FLASH_Unlock();
    status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    /*
    WRPERR:When a page is protected by WRP, it is not erased and the WRPERR bit is set.

    PGAERR:PGAERR is set if one of the following conditions occurs:
    – In standard programming: the first word to be programmed is not aligned with a
    double word address, or the second word doesn’t belong to the same double word
    address.
    – In fast programming: the data to program doesn’t belong to the same row than the
    previous programmed double words, or the address to program is not greater than
    the previous one.

    EOP: End of operation
    Set by hardware when one or more Flash memory operation (programming / erase) has
    been completed successfully.
    This bit is set only if the end of operation interrupts are enabled (EOPIE=1).
    Cleared by writing 1.
    */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR);
    FLASH_PageErase_L(page);
    status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
    HAL_FLASH_Lock();
}

void FLASH_Write_Test()
{

    uint32_t address = 0x0800F800UL;
    uint32_t u_data = 0x12345678UL;
    uint32_t page = GetPage(address);
    // /* 调用上述两个函数即可实现flash的擦除和写入 */
    // FLASH_Page_Erase(page);
    // HAL_StatusTypeDef ret = Flash_Write64(address, u_data);

    Flash_Write_Alignment64(address, (uint8_t*)&u_data, sizeof(u_data));
    /* 读取的话就简单了，直接使用地址即可 */
    uint32_t u_data1 = 0;
    u_data1 = *((uint32_t *)address);
}