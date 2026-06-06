#include "drivers_flash.h"
#include "gd32f4xx_hal.h"

int drivers_flash_erase_param_area(void)
{
    /* Erase parameter area: 0x08010000 size 4K */
    uint32_t PageAddress = 0x08010000;
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef eraseInit;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = PageAddress;
    eraseInit.NbPages = 1; /* may need adjust per page size */
    uint32_t SectorError = 0;
    if (HAL_FLASHEx_Erase(&eraseInit, &SectorError) != HAL_OK) {
        HAL_FLASH_Lock();
        return -1;
    }
    HAL_FLASH_Lock();
    return 0;
}

int drivers_flash_write(uint32_t addr, const uint8_t *data, uint32_t len)
{
    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < len; i += 4) {
        uint32_t word = 0xFFFFFFFF;
        for (uint32_t j = 0; j < 4 && (i+j) < len; j++) {
            word |= ((uint32_t)data[i+j]) << (8*j);
        }
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i, word) != HAL_OK) {
            HAL_FLASH_Lock();
            return -1;
        }
    }
    HAL_FLASH_Lock();
    return 0;
}

int drivers_flash_read(uint32_t addr, uint8_t *data, uint32_t len)
{
    memcpy(data, (const void*)addr, len);
    return 0;
}
