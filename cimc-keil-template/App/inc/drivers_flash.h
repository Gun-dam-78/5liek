#ifndef DRIVERS_FLASH_H
#define DRIVERS_FLASH_H
#include <stdint.h>
int drivers_flash_erase_param_area(void);
int drivers_flash_write(uint32_t addr, const uint8_t *data, uint32_t len);
int drivers_flash_read(uint32_t addr, uint8_t *data, uint32_t len);
#endif
