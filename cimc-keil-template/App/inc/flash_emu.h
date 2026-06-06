#ifndef FLASH_EMU_H
#define FLASH_EMU_H
#include <stdint.h>
int flash_params_store(const void *data, uint32_t len);
int flash_params_load(void *data, uint32_t len);
#endif
