#include "drivers_flash.h"
#include "gd32f4xx_hal.h"
#include <string.h>

/* Flash programming helpers for GD32F470 using HAL. Adjust page size/erase units according to device manual. */

/* Parameter area: 0x08010000 - 0x08010FFF (4K) */
#define PARAM_AREA_ADDR 0x08010000U
#define PARAM_AREA_SIZE 0x1000U

/* Staging area: 0x08051000 - 0x08070FFF (128K) */
#define STAGING_ADDR 0x08051000U
#define STAGING_SIZE 0x20000U

int drivers_flash_erase_param_area(void)
{
    HAL_FLASH_Unlock();
    uint32_t start = PARAM_AREA_ADDR;
    uint32_t end = PARAM_AREA_ADDR + PARAM_AREA_SIZE - 1;

    /* For GD32F4, erase by pages - assume page size 0x800 (2048 bytes) typical - adjust if different */
    const uint32_t page_size = 0x800U;
    for (uint32_t addr = start; addr <= end; addr += page_size) {
        FLASH_EraseInitTypeDef eraseInit;
        eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInit.PageAddress = addr;
        eraseInit.NbPages = 1;
        uint32_t SectorError = 0;
        if (HAL_FLASHEx_Erase(&eraseInit, &SectorError) != HAL_OK) {
            HAL_FLASH_Lock();
            return -1;
        }
    }
    HAL_FLASH_Lock();
    return 0;
}

int drivers_flash_write(uint32_t addr, const uint8_t *data, uint32_t len)
{
    HAL_FLASH_Unlock();
    /* Program in words */
    for (uint32_t i = 0; i < len; i += 4) {
        uint32_t word = 0xFFFFFFFF;
        for (uint32_t j = 0; j < 4 && (i+j) < len; j++) {
            word = (word & ~(0xFFu << (8*j))) | (((uint32_t)data[i+j]) << (8*j));
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

int drivers_flash_erase_staging(void)
{
    HAL_FLASH_Unlock();
    uint32_t start = STAGING_ADDR;
    uint32_t end = STAGING_ADDR + STAGING_SIZE - 1;
    const uint32_t page_size = 0x800U;
    for (uint32_t addr = start; addr <= end; addr += page_size) {
        FLASH_EraseInitTypeDef eraseInit;
        eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInit.PageAddress = addr;
        eraseInit.NbPages = 1;
        uint32_t SectorError = 0;
        if (HAL_FLASHEx_Erase(&eraseInit, &SectorError) != HAL_OK) {
            HAL_FLASH_Lock();
            return -1;
        }
    }
    HAL_FLASH_Lock();
    return 0;
}
